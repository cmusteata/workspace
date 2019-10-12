#include <iostream>
#include <vector>
#include <thread>

int compsum(const std::vector<int>& vect, int num)
{
    std::vector<int> temp(num);
    std::vector<std::thread> cont;

    auto singsum = [&](int id, int j, int len)
    {
        int sum = 0;
        for (int i = 0; i < len; ++i)
            sum += vect[j + i];
        temp[id] = sum;
    };

    int m = vect.size() % num;
    int n = vect.size() / num;
    for (int i = 0; i < num-1; ++i)
        cont.emplace_back(singsum, i, i*n, n);
    cont.emplace_back(singsum, num-1, (num-1)*n, n+m);

    // wait here for threads to complete and while
    // they complete add the result to the whole sum
    int sum = 0;
    for (int i = 0; i < num-1; ++i)
    {
        cont[i].join();
        std::cout << "thread " << i << ": " << temp[i] << std::endl;
        sum += temp[i];
    }

    cont[num-1].join();
    std::cout << "thread " << num-1 << ": " << temp[num-1] << std::endl;
    sum += temp[num-1];

    return sum;
};

int main()
{
    std::vector<int> myarr = {3,6,9,1,5,7,2,8,4,13,17,55,47,13,21,46,99,74,71,25};
    int sum = compsum(myarr, 3);
    std::cout << "sum: " << sum << std::endl;
    double avg = double(sum) / myarr.size();
    std::cout << "avg: " << avg << std::endl;
    return 0;
}
