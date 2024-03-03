#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <exception>

class thpool_except : public std::exception
{
    std::string _msg;

public:
    thpool_except();
    thpool_except(const std::string &msg);
    virtual const char *what() const noexcept override;
};

thpool_except::thpool_except() : _msg("Thread pool exception!"){};
thpool_except::thpool_except(const std::string &msg) : _msg(msg){};
const char *thpool_except::what() const noexcept
{
    return _msg.c_str();
};


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
    std::mutex m;
    std::condition_variable data_cond;
    std::queue<std::function<void()>> q;

public:
    void push(std::function<void()> func)
    {
        std::lock_guard<std::mutex> lock_guard(m);
        q.push(func);
        data_cond.notify_all();
    }

    std::function<void()> try_pop()
    {       
        if (q.empty())
            std::this_thread::yield();
        std::unique_lock<std::mutex> unique_lock(m);
        data_cond.wait(unique_lock, [&]()
            {  return !q.empty();
                 });  
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
    std::mutex m;
    size_t cores = 0;
    std::atomic<bool> exit{false};

public:
    thread_pool(size_t cores) : cores(cores)
    {
        if (cores > 100)
        {
            throw thpool_except("cores>100");
        }
        else
            threads_pool.resize(cores);

        threads_pool[0] = std::thread(&thread_pool::submit, this, func1);
        threads_pool[1] = std::thread(&thread_pool::submit, this, func2);


        for (size_t i = 2; i < cores; i++)
        {
            threads_pool[i] = std::thread(&thread_pool::work, this);
        }
    }

    ~thread_pool()
    {
        for (size_t i = 0; i < cores; i++)
        {
            if (i == 1)
            {
                exit = true;
            }
            threads_pool[i].join();
        }
    }

    void work()
    {
        while (true)
        {
            if (exit.load())
            {
                return;
            }   
            //std::lock_guard<std::mutex> lock_guard(m);
            auto func = que.try_pop();
            std::cout << std::this_thread::get_id() << std::endl;
            func();  
        } 
    }

    void submit(std::function<void()> func)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (int i = 0; i < 1; i++)
        {
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
    catch (const std::exception &ex)
    {
        std::cout << ex.what();
    }
    return 0;
}