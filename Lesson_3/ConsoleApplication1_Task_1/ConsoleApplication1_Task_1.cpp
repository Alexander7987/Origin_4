#include <iostream>
#include <vector>
#include <future>

void min(std::vector<int>& numbers, int size, int start, std::promise<int>& prom)
{
    int temp = 0;
    int min = numbers[start];
    for (int i = start; i < size; i++)
    {
        if (numbers[i] <= min)
        {
            temp = i;
            min = numbers[i];
        }
    }
    prom.set_value(temp);
}


void sort(std::vector<int>& numbers, int size)
{

    for (int i = 0; i < size; i++)
    {
        int k = 0; //сюда записываем индекс min элемента, как результат работы асинхронной функции по поиску min элемента
        int w = 0; // буфер для смены местами элементов

        std::promise<int> prom; //переопределяем переменные по новой, чтобы обновлялась связь между фьючер и промис, иначе пееременная k будет одним и тем же значением каждую новую итерацию
        std::future<int> f = prom.get_future();
        auto ff = std::async(min, std::ref(numbers), size, i, std::ref(prom));
        k = f.get();

        w = numbers[k];
        numbers[k] = numbers[i];
        numbers[i] = w;
    }

}


int main()
{
    srand(time(0));
    
    std::vector<int> numbers;
    int n;
    int k = 0;
    int w = 0;

    std::cout << "Enter size: ";
    std::cin >> n;
    numbers.resize(n);
    for (int i = 0; i < n; i++)
    {
        numbers[i] = 0 + rand() % 11;
    }

    for (auto& t : numbers)
    {
        std::cout << t << " ";
    }

    sort(numbers, n);

    std::cout << std::endl;
    for (auto& t : numbers)
    {
        std::cout << t << " ";
    }
    

    return 0;
}