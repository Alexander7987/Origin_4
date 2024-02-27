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
public:
    // TODO все эти атрибуты должны быть скрыты в private!
    std::mutex temp_mutex2; // TODO откуда такие названия? Можно же просто назвать std::mutex m
    std::condition_variable data_cond;
    std::queue<std::function<void()>> my_q; // TODO откуда такие названия? Можно же просто назвать q или que

    void push(std::function<void()> func)
    {
        std::lock_guard<std::mutex> lock_guard(temp_mutex2);
        my_q.push(func);
        data_cond.notify_all();
    }
    // TODO здесь в front и pop присутствует API race. Нужно объединить в один метод try_pop()
    //  и при возврате значений из метода использовать условные переменные data_cond
    //  для ожидания того что в очереди есть что вернуть
    std::function<void()> front()
    {
        std::lock_guard<std::mutex> lock_guard(temp_mutex2);
        return my_q.front();
    }

    void pop()
    {
        std::lock_guard<std::mutex> lock_guard(temp_mutex2);
        my_q.pop();
    }
};

class thread_pool : public safe_queue // TODO что это??? У вас уже есть композиция, зачем еще наследование?! Bad design
{
private:
    std::vector<std::thread> threads_pool;
    safe_queue temp_safe_queue; // TODO Откуда такой дикий нейминг? Можно же просто que
    std::mutex temp_mutex1;     // TODO bad naming

public:
    thread_pool(int cores) : threads_pool(cores) // TODO почему int? И что в этом случае будет если вам передадут cores < 0?
    {
        threads_pool[0] = std::thread(&thread_pool::submit, this, func1);
        threads_pool[1] = std::thread(&thread_pool::submit, this, func2);

        for (int i = 2; i < cores; i++)
        {
            threads_pool[i] = std::thread(&thread_pool::work, this);
        }
    }

    ~thread_pool()
    {
        for (int i = 0; i < 8; i++) // TODO откуда в цикле берется число 8? должно быть все параметризировано
        {
            // TODO вы никогда не дождетесь этих джоинов.
            // Нужно помимо джоинов здесь предусмотреть механизм оповещения выхода из вечных циклов
            threads_pool[i].join();
        }
    }

    void work()
    {
        while (true)
        {
            /*TODO
            1. нужно оповещение для выхода из вечного цикла, например с помощью атомарного булевого флага
            2. Блокировку при возврате значений делать в методе try_pop у safe_queue используя data_cond
            3. Должно выглядеть так:
             if (exit) {return};
             auto func = que.try_pop();
             std::lock_guard lk(m);
             func();
            */
            // TODO убрать все что следует ниже
            std::unique_lock<std::mutex> lock_guard(temp_mutex2);
            data_cond.wait(lock_guard, [&]()
                           { return !temp_safe_queue.my_q.empty(); });
            std::cout << std::this_thread::get_id() << std::endl;
            auto temp_func = temp_safe_queue.front();
            temp_func();
            lock_guard.unlock();
            temp_safe_queue.pop();
        }
    }

    void submit(std::function<void()> func)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (int i = 0; i < 3; i++)
        {
            std::lock_guard<std::mutex> lock_guard(temp_mutex1);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            temp_safe_queue.push(func);
        }
    }
};

int main()
{
    int cores = std::thread::hardware_concurrency();
    thread_pool my_object(cores);
    std::cout << "Cores: " << cores << std::endl;
    return 0;
}

/*


    my_object.my_threads[0] = std::thread(add1, std::ref(my_object));
    my_object.my_threads[1] = std::thread(add2, std::ref(my_object));

    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //std::cout << "df";
    my_object.my_threads[2] = std::thread(&thread_pool::work, &my_object);
    my_object.my_threads[3] = std::thread(&thread_pool::work, &my_object);





void add1(thread_pool my_object)
{
    for (int i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::lock_guard<std::mutex> lk(m1);
        my_object.submit(func1);
    }
}


void add2(thread_pool my_object)
{
    for (int i = 0; i < 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::lock_guard<std::mutex> lk(m1);
        my_object.submit(func2);
    }
}
*/