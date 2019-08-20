#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <ctime>
#include <cassert>
#include <fstream>

// struct BRIDGE
//
// This data-structure is used
// internally by the Yaml parser

struct BRIDGE
{
    int length;
    std::vector<double> paces;
};

// class Yaml
//
// This class is not intended to parse any generic .yaml file
// The sole purpose of this class is to parse the intro.yaml
//
// The following assumptions are made in order to parse:
// 1) The intro.yaml file is a valid yaml formatted file
// 2) All comments start at index 0
// 3) The sequence indicator is "- "
// 4) The keyvalue indicator is ": "
// 5) All sequences in a block are aligned relative to the same column
// 6) The underlying data structure of 'Yaml' is a sequence of BRIDGES
//    BRIDGE entries in the intro.yaml file start at index 0
// 7) Each BRIDGE has two key-value pairs starting at index 2
//    length: scalar-value
//    paces: list of scalar-values

class Yaml
{
    std::string fname;
    std::ifstream ifs;
    std::vector<BRIDGE> data;

    void parse(const std::string& line)
    {
        // ignore all comments
        if (line.find("# ") == 0)
            return;
        
        // looking for a sequence indicator
        std::size_t pos = line.find("- ");
        if (pos != std::string::npos)
        {
            if (pos == 0)
            {
                // emplace an empty  BRIDGE entry
                data.emplace_back();
                
                std::cerr << "[" << pos << "] " << "- " << std::endl;
            }
            else
            {
                assert(data.empty() == false);

                // emplace a new hiking-pace for this BRIDGE
                std::string value = line.substr(pos += 2);
                data.back().paces.emplace_back(std::stod(value));

                std::cerr << "[" << pos << "] " << "  - " << value << std::endl;
            }
        }

        // looking for a key-value indicator
        pos = line.find(": ");
        if (pos != std::string::npos)
        {
            const std::string key1 = "length";
            const std::string key2 = "paces";

            if (line.find(key1, 2) != std::string::npos)
            {
                assert(data.empty() == false);

                // sent the length of the bridge
                std::string value = line.substr(pos += 2);
                data.back().length = std::stoi(value);

                std::cerr << "[" << pos << "]   " << key1 << ": " << value << std::endl;
            }
            else if (line.find(key2, 2) != std::string::npos)
            {
                std::cerr << "[" << pos << "]   " << key2 << ": " << std::endl;
            }
        }
    }

public:
    Yaml(const std::string& filename)
    : fname(filename)
    , ifs(filename)
    {
    }

    ~Yaml()
    {
        ifs.close();
    }
    
    void parse()
    {
        assert(ifs.is_open() == true);

        std::string line;
        getline(ifs, line);
        assert(line == "---");

        while (getline(ifs, line))
            parse(line);
    }

    const std::vector<BRIDGE>& get() const
    {
        return data;
    }
};

// Data structure used internally by the 'class Algo'
// to compute the crossing distance & the crosing time

struct Bridge
{
    int length;
    double time;
 
    Bridge()
    : length(0)
    , time(0.0)
    {
    }

    Bridge(int len)
    : length(len)
    , time(0.0)
    {
    }

    const Bridge& operator +=(const Bridge& bridge)
    {
        length += bridge.length;
        time   += bridge.time;
        return *this;
    }

    friend std::ostream& operator <<(std::ostream& os, const Bridge& bridge)
    {
        os << "[" << bridge.length << " ft, " << bridge.time << " min]";
        return os;
    }
};

// This Algo computes the crossing time and total distance
// The assumption is made that a faster hiker on the right side of the bridge
// can't help the hikers on the left side to improve the crossing time.
// Each group on the left side independently has to cross the bridge and
// then join the group of hikers on the right side.

class Algo
{
    Bridge result;
    std::vector<Bridge> bridges;
    std::vector<std::vector<double>> groups;

    void show() const
    {
        for (size_t k = 0; k < groups.size(); ++k)
        {
            for (double pace : groups[k])
                std::cerr << pace << ',';
            if (k < bridges.size())
                std::cerr << bridges[k] << ' ';
        }
        std::cerr << std::endl;
    }

public:
    Algo(const Yaml& yaml)
    {
        for (const auto& y : yaml.get())
        {
            bridges.emplace_back(y.length);
            groups.emplace_back();
            auto& group = groups.back();
            for (double pace : y.paces)
                group.push_back(pace);
        }

        // add [n + 1] group to help transfer
        // the hikers across the [n] bridge
        groups.emplace_back();
    }

    void run()
    {
        Bridge total;
        for (size_t k = 0; k < bridges.size(); ++k)
        {
            auto& lhs = groups[k];   // left  hand side of the bridge  
            auto& rhs = groups[k+1]; // right hand side of the bridge
            std::sort(lhs.begin(), lhs.end(), std::greater<double>());
            show();

            // The top 2 fastest hikers on the left side cross together
            // while the fastest on the left side returns back to handover
            // the torch to the slowest pair of hikers on the left side.
            // The slowest pair of hikers cross the bridge then the second
            // fastest returns the torch back to the left side group of hikers.
            // 
            // Note that the fastest pair of hikers always return back so
            // only the timing is incremented while not necessary to phisically
            // move the hikers from one container to another back and forth.
            // However, for the slowest pair of hikers we both increment the time
            // and phisically move the hikers to the next container.
            // 
            // We repeat this same algorithm for each bridge by removing the
            // slowest pair of hikers at each iteration until 1, 2 or 3 hikers
            // are left in the group. From there, there's no particular variation
            // to improve any timing therefore we consider them particular cases.
            //
            // The hikers are represented by their pace and we sort them before
            // crossing each bridge. The choice of using a std::vector to move
            // the hikers and std::sort sorting algorithm with [n*log(n)] time
            // complexity potentially may be faster than other data-structures
            // (have to prove) for a reasonable numbers of hikers in each group
            // as long as each group fits in a line of CPU cache in order to
            // avoid false sharing at sorting.

            auto& bridge = bridges[k];
            for (size_t n = lhs.size(); n > 0; n = lhs.size())
            {
                if (n == 1)
                {
                    // one-way trip
                    // timing for hiker [0]
                    bridge.time += bridge.length / lhs[0];

                    // transfer for the last hikers
                    while (!lhs.empty())
                    {
                        rhs.push_back(lhs.back());
                        lhs.pop_back();
                    }
                }
                else if (n == 2)
                {
                    // one-way trip
                    // timing for hikers [0] and [1]
                    bridge.time += bridge.length / lhs[1];

                    // transfer for the last hikers
                    while (!lhs.empty())
                    {
                        rhs.push_back(lhs.back());
                        lhs.pop_back();
                    }
                }
                else if (n == 3)
                {
                    // timing for the fastest pair of hikers from the
                    // left side lhs[0] & lhs[1] at the pace of lhs[1]
                    bridge.time += bridge.length / lhs[1];

                    // timing for the fastest hiker from the left side
                    // who just returned the torch from the right side
                    bridge.time += bridge.length / lhs[0];

                    // timing for the hikers lhs[2] & lhs[0] at the pace of lhs[2]
                    bridge.time += bridge.length / lhs[2];

                    // move all hikers from the left to the right side
                    while (!lhs.empty())
                    {
                        rhs.push_back(lhs.back());
                        lhs.pop_back();
                    }
                }
                else
                {
                    // timing for the fastest pair of hikers from the
                    // left side lhs[0] & lhs[1] at the pace of lhs[1]
                    bridge.time += bridge.length / lhs[1];

                    // timing for the fastest hiker from the left side
                    // who just returned the torch from the right side
                    bridge.time += bridge.length / lhs[0];

                    // timing for the slowest pair of hikers from the
                    // left side lhs[j] and lhs[j-1] at the pace of lhs[j]
                    size_t j = n - 1;
                    bridge.time += bridge.length / lhs[j];

                    // timing for the second fastest hiker from the left side
                    // who just returned the torch from the right side
                    bridge.time += bridge.length / lhs[1];

                    // move the slowest pair of hikers from left to the right side
                    for (int i = 0; i < 2; ++i)
                    {
                        rhs.push_back(lhs.back());
                        lhs.pop_back();
                    }
                }

                // display the progress as we transfer the hikers
                show();
            }

            // total crossing all bridges
            total += bridge;
        }

        // store the result
        result = total;
    }

    friend std::ostream& operator <<(std::ostream& os, const Algo& algo)
    {
        for (size_t k = 0; k < algo.bridges.size(); ++k)
            os << "bridge " << k << ": " << algo.bridges[k] << std::endl;
        os << "overall : " << algo.result;
        return os;
    }
};

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

