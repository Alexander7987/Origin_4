#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>

void client(int max_counter_client, std::atomic<int>& counter_client)
{
    for (int i = 0; i < max_counter_client; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        counter_client++;
        std::cout << "CLIENT. ID thread: " << std::this_thread::get_id() << "    \tCurrent amount clients: " << counter_client.load(std::memory_order_seq_cst);
        std::cout << std::endl;
    }
}

void client_operator(int max_counter_client, std::atomic<int>& counter_client)
{
    for (int i = 0; i < max_counter_client; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        counter_client--;
        std::cout << "OPERATOR. ID thread: " << std::this_thread::get_id() << "    \tCurrent amount clients: " << counter_client.load(std::memory_order_seq_cst);
        std::cout << std::endl;
    }
}

int main()
{
    int max_counter_client = 0;
    std::atomic<int> counter_client = 0;
    std::cout << "Enter max clients: ";
    std::cin >> max_counter_client;
    std::thread t1(client, max_counter_client, std::ref(counter_client));
    std::thread t2(client_operator, max_counter_client, std::ref(counter_client));
    t1.join();
    t2.join();
    return 0;
}