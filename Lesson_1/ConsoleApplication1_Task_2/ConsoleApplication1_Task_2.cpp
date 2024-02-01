#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>


void sum(std::vector<int> vector_1, std::vector<int> vector_2, std::vector<int>& vector_3, int num_thread, int count, int tempp) // num_thread - номер текущего потока
{
    int q = 0;

    for (q = (num_thread - 1) * count; q < tempp; q++) // Цикл записи результатов сложения в третий вектор. Допустим, если 4 потока, то 0-249(включительно), далее 250-499(включительно) и т.д.
    {
        vector_3[q] = (vector_1[q] + vector_2[q]);
    }
    int y = 0;
}


int main()
{
    std::mt19937 gen;
    std::uniform_int_distribution<int> dis(0, 1000);
    auto rand_num([=]() mutable {return dis(gen); });
    std::vector<double> result_time;
    std::vector<int> vector_1;
    std::vector<int> vector_2;
    std::vector<int> vector_3; //пустой вектор для записи результатов суммы
    std::vector<std::thread> vector_threads; //вектор потоков

    int count = 0; //сколько будет операций "сложения" в ОДНОМ потоке 
    int q = 0; //текущий счетчик операций сложения. Он считает "отрезками" и зависит от tempp. Если принимать amount = 1000, то сначала 0-249(включительно), 250-499(включительно) и т.д. 
    int tempp = 0; //после прохождения очередного отрезка, необходимо запоминать, где остановился счетчик "q".


    std::cout << "Amount available of cores - " << std::thread::hardware_concurrency() << std::endl << std::endl;
    int number_thread = 1;


    for (int j = 1000; j <= 1000000; j *= 10) //цикл изменения кол-ва элементов
    {
        vector_1.clear();
        vector_2.clear();
        vector_1.resize(j);
        vector_2.resize(j);

        std::generate(std::begin(vector_1), std::end(vector_1), rand_num); // заполняем числами 1 вектор
        std::generate(std::begin(vector_2), std::end(vector_2), rand_num); // заполняем числами 2 вектор               

        for (int n = 1; n <= 5; n++) // 1 2 4 8 16 - итого 5 вариантов количества потоков для каждой размерности векторов. Например, для 1000 проходим 5 раз, и так для каждого: 10000, 100000, 1000000
        {
            if (j == 1000 && n == 5) // т.к. 1000%16 !=0, то создаем особое условие для этого случая (когда имеем 16 потоков)
            {
                count = 62;
            }

            vector_3.clear(); //очищаем перед входом в новый вариант количества потоков
            vector_3.resize(j);


            switch (n)
            {
            case 1:
            {
                tempp = 0; // обнуляем после смены количества потоков
                count = j / 1; // сколько будет операций "сложения" в ОДНОМ потоке 
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 1; i <= 1; i++)// цикл изменения кол-ва потоков для определенного числа элементов
                {
                    tempp += count;
                    vector_threads.push_back(std::thread(sum, vector_1, vector_2, std::ref(vector_3), i, count, tempp));
                }
                for (auto& t : vector_threads)
                {
                    t.join();
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                result_time.push_back(duration.count());
                vector_threads.clear();
                break;
            }


            case 2:
            {
                tempp = 0; // обнуляем после смены количества потоков
                count = j / 2; // сколько будет операций "сложения" в ОДНОМ потоке 
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 1; i <= 2; i++)// цикл изменения кол-ва потоков для определенного числа элементов
                {
                    tempp += count;
                    vector_threads.push_back(std::thread(sum, vector_1, vector_2, std::ref(vector_3), i, count, tempp));
                }
                for (auto& t : vector_threads)
                {
                    t.join();
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                result_time.push_back(duration.count());
                vector_threads.clear();
                break;
            }


            case 3:
            {
                tempp = 0; // обнуляем после смены количества потоков
                count = j / 4; // сколько будет операций "сложения" в ОДНОМ потоке 
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 1; i <= 4; i++)// цикл изменения кол-ва потоков для определенного числа элементов
                {
                    tempp += count;
                    vector_threads.push_back(std::thread(sum, vector_1, vector_2, std::ref(vector_3), i, count, tempp));
                }
                for (auto& t : vector_threads)
                {
                    t.join();
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                result_time.push_back(duration.count());
                vector_threads.clear();
                break;
            }


            case 4:
            {
                tempp = 0; // обнуляем после смены количества потоков
                count = j / 8; // сколько будет операций "сложения" в ОДНОМ потоке 
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 1; i <= 8; i++)// цикл изменения кол-ва потоков для определенного числа элементов
                {
                    tempp += count;
                    vector_threads.push_back(std::thread(sum, vector_1, vector_2, std::ref(vector_3), i, count, tempp));
                }
                for (auto& t : vector_threads)
                {
                    t.join();
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                result_time.push_back(duration.count());
                vector_threads.clear();
                break;
            }


            case 5:
            {
                tempp = 0; // обнуляем после смены количества потоков
                count = j / 16; // сколько будет операций "сложения" в ОДНОМ потоке 
                auto start = std::chrono::high_resolution_clock::now();
                for (int i = 1; i <= 16; i++)// цикл изменения кол-ва потоков для определенного числа элементов
                {
                    if (i == 16 && count == 62) // т.к. 1000%16 !=0, то создаем особое условие для этого случая (когда имеем 16 потоков)
                    {
                        tempp = 1000;
                    }
                    else
                        tempp += count;
                    vector_threads.push_back(std::thread(sum, vector_1, vector_2, std::ref(vector_3), i, count, tempp));
                }
                for (auto& t : vector_threads)
                {
                    t.join();
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                result_time.push_back(duration.count());
                vector_threads.clear();
                break;
            }
            }
        }
    }
    std::cout << "                  \t1000      \t10000     \t100000     \t1000000" << std::endl;


    for (int i = 0; i < 5; i++) //выводим результаты
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