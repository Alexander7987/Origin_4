#include <iostream>
#include <thread>
#include <vector>
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
    std::mutex m2;
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
        return func;
    }
};

class thread_pool
{
private:
    std::vector<std::thread> threads_pool;
    safe_queue que;
    std::mutex m1;
    size_t cores = 0;
    std::atomic<bool> flag_to_exit = false;

public:
    thread_pool(size_t cores) : cores(cores)
    {
        if (cores > 40) // а какое число взять, разумное для ограничения?
        {
            throw std::exception();
        }
        else
            threads_pool.resize(cores);

        threads_pool[0] = std::thread(&thread_pool::submit, this, func1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        threads_pool[1] = std::thread(&thread_pool::submit, this, func2);

        for (size_t i = 2; i < cores; i++)
        {
            threads_pool[i] = std::thread(&thread_pool::work, this);
        }
    }

    ~thread_pool()
    {
        for (int i = 0; i < cores; ++i)
        {
            if (i == 1)
            {
                flag_to_exit = true; // первые два потока добавляют функции в очередь. При окончании работы последнего (вторым, в данном случае) потоком ставим флаг в true;
            }
            threads_pool[i].join();
        }
    }

    void work()
    {       
        while (true)
        {
            if (flag_to_exit)
            {
                return;
            }
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
    catch (const std::exception& ex)
    {
        std::cout << ex.what();
    }
    return 0;
}
//