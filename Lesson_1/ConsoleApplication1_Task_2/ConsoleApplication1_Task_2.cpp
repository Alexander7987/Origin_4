#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>


void sum(std::vector<int> vector_1, std::vector<int> vector_2, const int& j, const int& amount, std::vector<double>& result_time) //j - количество потоков
{
    std::vector<std::thread> vector_threads;
    std::vector<int> vector_3(amount); //пустой вектор для записи результатов суммы, но он необязателен
    int count = 0; //сколько будет операций "сложения" в ОДНОМ потоке 
    int q = 0; //текущий счетчик операций сложения. Он считает "отрезками" и зависит от tempp. Если принимать amount = 1000, то сначала 0-249(включительно), 250-499(включительно) и т.д. 
    int tempp = 0; //после прохождения очередного отрезка, необходимо запоминать, где остановился счетчик "q".
    if (amount == 1000 && j == 16) // т.к. 1000%16 !=0, то создаем особое условие для этого случая (когда имеем 16 потоков)
    {
        count = 62;
    }
    else
        count = amount / j;    

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < j; i++)   //  0/16   15/16
    {
        vector_threads.push_back(std::thread([&i, &count, &q, &tempp, &vector_1, &vector_2, &vector_3]() {
            if (i == 15 && count == 62) // т.к. 1000%16 !=0, то создаем особое условие для этого случая (когда имеем 16 потоков)
            {
                tempp = 1000;
            }
            else
                tempp += count;
            auto iter = vector_3.cbegin();
            q = 0;
            for (q = i * count; q < tempp; q++) // Цикл записи результатов сложения в третий вектор. Допустим, если 4 потока, то 0-249(включительно), далее 250-499(включительно) и т.д.
            {
                if (q == 0) // т.к. добавление через emplase идет ПОСЛЕ обозначенного элемента если указываем как "iter + n", где n!=0, то для n!=0 указываем "q-1". 
                    vector_3.emplace(iter, vector_1[q] + vector_2[q]);
                else
                    vector_3.emplace(iter + q, vector_1[q] + vector_2[q]);
            }
            }));
        //такие сложные условия и сам цикл из-за того, что потоки будут работать одновременно, и один не будет ждать, пока другой закончит. Поэтому, в цикле предусмотрено 
        //заполнение каждого кусочка вектора параллельно в условиях мнгопоточности
    }
    for (auto& t : vector_threads)
    {
        t.join();
    }

    vector_threads.clear();
    vector_3.clear();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    result_time.push_back(duration.count());
}


int main()
{
    std::mt19937 gen;
    std::uniform_int_distribution<int> dis(0, 1000);
    std::vector<double> result_time;
    std::vector<int> vector_1;
    std::vector<int> vector_2;
    std::cout << "Amount available of cores - " << std::thread::hardware_concurrency() << std::endl << std::endl;
    int number_thread = 1;


    for (int j = 1000; j <= 1000000; j *= 10) //цикл изменения кол-ва элементов
    {
        vector_1.clear();
        vector_2.clear();
        vector_1.resize(j);
        vector_2.resize(j);
        auto rand_num([=]() mutable {return dis(gen); });
        std::generate(std::begin(vector_1), std::end(vector_1), rand_num); // заполняем числами вектор
        std::generate(std::begin(vector_2), std::end(vector_2), rand_num); // заполняем числами вектор
        for (int i = 1; i <= 16; i *= 2) // цикл изменения кол-ва потоков для определенного числа элементов
        {
            sum(vector_1, vector_2, i, j, result_time); // вызываем функцию
        }
    }

    std::cout << "                  \t1000      \t10000     \t100000     \t1000000" << std::endl;

    for (int i = 0; i < 5; i++)
    {
        std::cout << number_thread << " threads\t";
        std::cout << "\t" << result_time.at(i) << "";
        std::cout << "\t" << result_time.at(i + 5) << "";
        std::cout << "\t" << result_time.at(i + 10) << "";
        std::cout << "\t" << result_time.at(i + 15) << "" << std::endl;
        number_thread *= 2;
    }

    return 0;
}





