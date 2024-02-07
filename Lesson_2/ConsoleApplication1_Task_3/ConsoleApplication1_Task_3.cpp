#include<iostream>
#include<mutex>


class Data
{
public:
    std::mutex m;
    int temp_int = 0;
    int* temp_point = nullptr;
    double temp_double = 0.0;

    Data(const int& a)
    {
        temp_int = a;
    }

    Data& operator = (Data&& other) noexcept
    {
        if (this == &other)
            return *this;
        temp_int = other.temp_int;

        delete temp_point;
        temp_point = other.temp_point;
        other.temp_point = nullptr;

        return *this;
    }

    ~Data()
    {
        delete temp_point;
    }
};


void swap_1(Data& var_1, Data& var_2)
{
    std::lock(var_1.m, var_2.m);
    Data temp(9);
    temp = std::move(var_1);
    var_1 = std::move(var_2);
    var_2 = std::move(temp);
    var_1.m.unlock();
    var_2.m.unlock();
}

void swap_2(Data& var_1, Data& var_2)
{
    std::scoped_lock sk_1(var_1.m, var_2.m);
    Data temp(9);
    temp = std::move(var_1);
    var_1 = std::move(var_2);
    var_2 = std::move(temp);
}

void swap_3(Data& var_1, Data& var_2)
{
    std::unique_lock ul_1(var_1.m, std::defer_lock);
    std::unique_lock ul_2(var_2.m, std::defer_lock);
    std::lock(ul_1, ul_2);
    Data temp(9);
    temp = std::move(var_1);
    var_1 = std::move(var_2);
    var_2 = std::move(temp);
    ul_1.unlock();
    ul_2.unlock();
}




int main()
{
    Data var_1(4);
    Data var_2(6);
    std::cout << "BEFORE var_1: " << var_1.temp_int << std::endl;
    std::cout << "BEFORE var_2: " << var_2.temp_int << std::endl;
    swap_1(var_1, var_2);
    swap_2(var_1, var_2);
    swap_3(var_1, var_2);
    std::cout << std::endl << "AFTER var_1: " << var_1.temp_int << std::endl;
    std::cout << "AFTER var_2: " << var_2.temp_int << std::endl;
    return 0;
}
