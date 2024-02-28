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
        data_cond.wait(unique_lock, [&]() {
            return !q.empty(); });
        auto func = q.front();
        q.pop();
        unique_lock.unlock();
        return func;
    }
};



class thread_pool
{
private:
    std::vector<std::thread> threads_pool;
    safe_queue que;
    std::mutex m1;
    double cores = 0;
    bool flag = false;

public:
    thread_pool(double cores) : cores(cores)
    {
        if (cores < 0)
        {
            throw "bad index";
        }
        else
            threads_pool.resize(cores);

        threads_pool[0] = std::thread(&thread_pool::submit, this, func1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        threads_pool[1] = std::thread(&thread_pool::submit, this, func2);

        for (int i = 2; i < cores; i++)
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
            if (flag)
            {
                break;
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
        flag = true;
    }
};



int main()
{
    int cores = std::thread::hardware_concurrency();
    std::cout << "Cores: " << cores << std::endl;
    thread_pool my_object(cores);
    return 0;
}