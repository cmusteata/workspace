#include <iostream>
#include <chrono>
#include <ctime>
#include <cassert>
#include "yaml.h"
#include "algo.h"

int main(int argc, char* argv[])
{
    assert(argc == 2);
    std::string filename = argv[1];

    std::cout << "open file: " << filename << std::endl;

    auto begin = std::chrono::system_clock::now();
    time_t begin_time = std::chrono::system_clock::to_time_t(begin);

    Yaml yaml(filename);
    yaml.parse();

    Algo algo(yaml);
    algo.run();

    std::cout << algo << std::endl;

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::chrono::duration<double> elapsed_seconds = end - begin;

    std::cout << "begin time: " << std::ctime(&begin_time);
    std::cout << "  end time: " << std::ctime(&end_time);
    std::cout << "   elapsed: " << elapsed_seconds.count() << " seconds"  << std::endl;

    return 0;
}

