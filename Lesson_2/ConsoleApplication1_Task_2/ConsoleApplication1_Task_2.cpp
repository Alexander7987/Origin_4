#include<iostream>
#include<random>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<Windows.h>
#include<condition_variable>

class consol_parameter
{
public:
    static void SetColor(int text, int background)
    {
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }
    static void SetPosition(int x, int y)
    {
        COORD point;
        point.X = x;
        point.Y = y;
        SetConsoleCursorPosition(hStdOut, point);
    }
private:
    static HANDLE hStdOut;
};
HANDLE consol_parameter::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

std::mutex m;

void func(int i, int temp)
{
    int j = 0;
    auto start = std::chrono::high_resolution_clock::now(); 
    m.lock();
    consol_parameter::SetPosition(0, i + 2);
    std::cout << i << "   " << std::this_thread::get_id() << "  ";      
    m.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for (j = 0; j < 25; j++)
    {           
        m.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(temp));
        consol_parameter::SetPosition(j + 11, i + 2);
        std::cout << "@";
        m.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }  
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    m.lock();    
    consol_parameter::SetPosition(j + 11, i + 2);
    std::cout << "   " << duration.count() << std::endl;
    m.unlock();  
}

int main()
{
    std::srand(time(0));
    int temp = 0;
    int count_thread = -1;
    int amount_threads = 0;
    std::vector<std::thread> vector_threads;
    std::cout << "Enter amount threads: ";
    std::cin >> amount_threads;
    std::cout << "#   id           Progress Bar           Time" << std::endl;
    for (int i = 0; i < amount_threads; i++)
    {     
        temp = 10 + rand() % 50;
        vector_threads.push_back(std::thread(func, i, temp));
    }  
    for (auto& t : vector_threads)
    {
        t.join();      
    }
    for (int y = 0; y < amount_threads; y++)
    {
        std::cout << std::endl;
    }
    //std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
    return 0;
}
