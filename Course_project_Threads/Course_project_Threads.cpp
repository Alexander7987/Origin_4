#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <queue>
#include <condition_variable>

std::mutex m1;
std::mutex m2;
std::condition_variable data_cond2;
bool flag = false;

void func1()
{
    static int t = 0;
    std::cout << "Working: " << __FUNCTION__ << " " << t++ << " " << std::endl;
}


void func2()
{
    std::cout << "Working: " << __FUNCTION__ << std::endl;
}


class safe_queue
{
public:
    std::queue<std::function<void()>> my_q;
    std::condition_variable data_cond;

    void push(std::function<void()> func)
    {
        std::lock_guard<std::mutex> lkk(m2);
        my_q.push(func);
        //data_cond.notify_all();
    }

    /*
    std::function<void()> front()
    {
        std::lock_guard<std::mutex> lkk(m2);
        return my_q.front();
    }
    */

    void pop()
    {
        std::unique_lock<std::mutex> lkk(m2);
        //data_cond.wait(lkk, [&]() {
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
        for (int i = 0; i < 4; i++)
        {
            my_threads[i].join();
        }
        my_threads.clear();
    }

    void work()
    {
        std::unique_lock<std::mutex> lk(m1);
        data_cond2.wait(lk, [&]() {
            return !my_queue.my_q.empty();
            });
        while (!my_queue.my_q.empty())
        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(200));             
            std::cout << std::this_thread::get_id() << std::endl;
            auto& temp_func = my_queue.my_q.front();//my_queue.front();
            temp_func();
            my_queue.pop();
        }
    }

    void submit(std::function<void()> func)
    {
        my_queue.push(func);
    }
};


void add1(thread_pool my_object)
{
    for (int i = 0; i < 3; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::lock_guard<std::mutex> lk(m1);
        my_object.submit(func1);
    }
    data_cond2.notify_all();
}


void add2(thread_pool my_object)
{
    for (int i = 0; i < 3; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::lock_guard<std::mutex> lk(m1);
        my_object.submit(func2);
    }
    data_cond2.notify_all();
}

int main()
{
    int cores = std::thread::hardware_concurrency();
    thread_pool my_object(cores);
    std::cout << "Cores: " << cores << std::endl;
    
    my_object.my_threads[0] = std::thread(add1, &my_object);
    //my_object.my_threads[1] = std::thread(add2, &my_object);
     
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));

    my_object.my_threads[2] = std::thread(&thread_pool::work, &my_object);
    //my_object.my_threads[3] = std::thread(&thread_pool::work, &my_object);

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