#include<iostream>
#include<random>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<Windows.h>

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


void func(int& count_thread)
{
    
    std::srand(time(0));
    int temp = 10 + rand() % 200;
    count_thread++;
    static int tempp = 1;
    tempp++;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << count_thread << "   " << std::this_thread::get_id() << "  ";
    consol_parameter::SetPosition(11, tempp);
    for (int i = 0; i < 25; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(temp));      
        std::cout << "@";      
    }    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "  " << duration.count() << std::endl;
}

int main()
{
    int count_thread = -1;
    int amount_threads = 0;
    std::vector<std::thread> vector_threads;
    std::cout << "Enter amount threads: ";
    std::cin >> amount_threads;
    std::cout << "#   id           Progress Bar           Time" << std::endl;
    for (int i = 0; i < amount_threads; i++)
    {       
        vector_threads.push_back(std::thread(func, std::ref(count_thread)));
    }
    for (auto& t : vector_threads)
    {
        t.join();
    }
    return 0;
}
