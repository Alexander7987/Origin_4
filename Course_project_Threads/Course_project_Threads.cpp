#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <queue>
#include <condition_variable>

void func1()
{
    std::cout << "Working: " << __FUNCTION__ << " " << std::endl;
}

void func2()
{
    std::cout << "Working: " << __FUNCTION__ << std::endl;
}

class safe_queue
{
private:
    std::mutex m2; // TODO почему m2? Разве есть m1 или m0?
    std::condition_variable data_cond;
    std::queue<std::function<void()>> q;

public:
    void push(std::function<void()> func)
    {
        std::lock_guard<std::mutex> lock_guard(m2);
        q.push(func);
        data_cond.notify_all();
    }

    std::function<void()> try_pop()
    {
        std::unique_lock<std::mutex> unique_lock(m2);
        data_cond.wait(unique_lock, [&]()
                       { return !q.empty(); });
        auto func = q.front();
        q.pop();
        unique_lock.unlock(); // TODO unlock не обязателен, у вас он и так вызовится при выходе из функции
        return func;
    }
};

class thread_pool
{
private:
    std::vector<std::thread> threads_pool;
    safe_queue que;
    std::mutex m1;     // TODO почему m1? Разве есть m2 или m0?
    double cores = 0;  // TODO почему double? у вас что число ядер может быть дробное?!:) Нужно использовать size_t, либо если int то проверку на < 0
    bool flag = false; // TODO не говорящее название флага

public:
    thread_pool(double cores) : cores(cores)
    {
        if (cores < 0)
        {
            throw "bad index"; // TODO такой тип исключения не используют! Если хотите бросить исключение, создайте свой класс исключения отнаследовавшись от std::exception
        }
        else
            threads_pool.resize(cores); // TODO а если cores > 100_000_000_000 вы выделите 100ГБ памяти?

        threads_pool[0] = std::thread(&thread_pool::submit, this, func1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        threads_pool[1] = std::thread(&thread_pool::submit, this, func2);

        for (int i = 2; i < cores; i++) // TODO неявное приведение типов с таким типом cores! Никогда так не делайте
        {
            threads_pool[i] = std::thread(&thread_pool::work, this);
        }
    }

    ~thread_pool()
    {
        for (int i = 0; i < cores; i++)
        {
            threads_pool[i].join();
        }
    }

    void work()
    {
        while (true)
        {
            if (flag) // DATA RACE. Флаг должен быть атомарным!
            {
                return;
            }
            std::lock_guard<std::mutex> lock_guard(m1);
            auto func = que.try_pop();
            std::cout << std::this_thread::get_id() << std::endl;
            func();
        }
    }

    void submit(std::function<void()> func)
    {
        for (int i = 0; i < 3; i++)
        {
            std::lock_guard<std::mutex> lock_guard(m1);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            que.push(func);
        }
        flag = true; // TODO BAD DESIGN. Флаг надо устанавливать в деструкторе
    }
};

int main()
{
    int cores = std::thread::hardware_concurrency();
    std::cout << "Cores: " << cores << std::endl;
    try
    {
        thread_pool my_object(cores);
    }
    catch (std::string e) // TODO вы бросаете здесь тип const char* и не отлавливаете его, соответственно будет abort если возникнет исключение
    {
        std::cout << e;
    }
    return 0;
}