#include <iostream>
#include <vector>
#include <future>

std::string func()
{
    return "do something in func";
}

template <class T, class TT>
std::string my_for_each(T begin, T end, TT func)
{
    auto size = std::distance(begin, end);
    if (size <= 2) //делим на 3 блока, т.е. будет по 2 эл-та в каждом
    {
        return func();
    }
    auto mid = begin;
    std::advance(mid, size / 2);
    auto f1 = std::async(my_for_each<T, TT>, begin, mid, func);
    auto f2 = my_for_each(mid, end, func);
    return f1.get() + "  +  " + f2;
}


int main()
{
    std::vector<int> my_vector{ 7, 2, 9, 3, 5, 1, 4, 0 };
    int y = 0;

    std::promise<int> prom; 
    auto f = prom.get_future();

    std::cout << my_for_each(my_vector.begin(), my_vector.end(), func);

    return 0;
}

//                       7 2 9 3 5 1 4 0
//                             /\
//                            /  \
//                           /    \
//                          /      \
//                         /        \
//                    7 2 9 3  +  5 1 4 0     
//                        /\         /\
//                       /  \       /  \
//                    7 2   9 3   5 1  4 0
//                   (do + do)   (do  +  do)