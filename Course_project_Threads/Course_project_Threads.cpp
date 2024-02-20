#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <queue>
#include <condition_variable>

std::mutex m1;
std::mutex m2;

void func1()
{
    std::cout << "Working: " << __FUNCTION__ << std::endl;
}


void func2()
{
    std::cout << "Working: " << __FUNCTION__ << std::endl;
}


class safe_queue
{
    std::queue<std::function<void()>> my_q;
    std::condition_variable data_cond;
public:
    void push(std::function<void()> func)
    {
        std::lock_guard<std::mutex> lk(m1);
        my_q.push(func);
       //data_cond.notify_all();
    }

    std::function<void()> front()
    {
        std::lock_guard<std::mutex> lk(m1);
        return my_q.front();
    }

    void pop()
    {
        std::unique_lock<std::mutex> lk(m1);
       // data_cond.wait(lk, [&]() {
            //return !my_q.empty();
            //});
        my_q.pop();
    }
};



class thread_pool
{ 
public:   
    std::vector<std::thread> my_threads;
    safe_queue my_queue;

    thread_pool(int cores)
    {
        my_threads.resize(cores);
    }

    ~thread_pool()
    {
        for (auto& t : my_threads)
        {
            t.join();
        }
        my_threads.clear();
    }

    void work()
    {
        std::cout << std::this_thread::get_id() << std::endl;
        auto temp_func = my_queue.front();
        temp_func();
        my_queue.pop();
    }

    void submit(std::function<void()> func)
    {
        my_queue.push(func);
    }
};



int main()
{
    int cores = std::thread::hardware_concurrency();
    thread_pool my_object(cores);
    std::cout << "Cores: " << cores << std::endl;

    std::thread t1([&]() {
        for (int i = 0; i < 10; i++)
        {           
            my_object.submit(func1);
            my_object.submit(func2);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        });

    for (int i = 0; i < 2; i++)
    {
        my_object.my_threads.push_back(std::thread(&thread_pool::work, &my_object));
    }


    t1.join();

    return 0;
}





/*
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