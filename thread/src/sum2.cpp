#include <iostream>
#include <vector>
#include <thread>
#include <cassert>

int compsum(const std::vector<int>& vect, int num)
{
    auto singsum = [&](int id, int* sum)
    {
        for (size_t i = id; i < vect.size(); i += num)
            *sum += vect[i];
    };

    std::vector<int> temp(num);
    std::vector<std::thread> cont(num);
    for (int i = 0; i < num; ++i)
        cont[i] = std::thread(singsum, i, &temp[i]);

    // wait here for threads to complete and while
    // they complete add the result to the whole sum
    int sum = 0;
    for (int i = 0; i < num; ++i)
    {
        cont[i].join();
        std::cout << "thread " << i << ": " << temp[i] << std::endl;
        sum += temp[i];
    }

    return sum;
};

std::vector<int> myarr = {
     3, 6, 9, 1, 5, 7, 2, 8, 4,13,
    17,55,47,13,21,46,99,74,71,25
};

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int n = std::stoi(argv[1]);
    
    int sum = compsum(myarr, n);
    std::cout << "sum: " << sum << std::endl;
    
    double avg = double(sum) / myarr.size();
    std::cout << "avg: " << avg << std::endl;
    
    return 0;
}
