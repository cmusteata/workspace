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
// The assumption is made that a faster hiker in the [k + 1] group
// can't help the hikers in [k] group to improve the crossing time
// Each group [k] independently has to cross the bridge and then
// join the [k+1] group

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
            std::sort(groups[k].begin(), groups[k].end(), std::greater<double>());
            show();

            // The top 2 fastest hikers in the first group cross together
            // while the fastest in the first group returns back to handover
            // the torch to the bottom 2 slowest hikers in the first group.
            // The bottom 2 slowest hikers cross the bridge while the second
            // fastest brings the torch back to the initial group of hikers.
            // 
            // Note that the top 2 fastest hikers always return back so
            // only the timing is incremented while not necessary to phisically
            // move the hikers from one container to another back and forth.
            // While for the bottom 2 slowest hikers we both increment the timer
            // and phisically move the hikers to the next container.
            // 
            // I repeat this same algorithm for each bridge by removing the
            // bottom 2 slowest hikers at each iteration until 1, 2 or 3 hikers
            // are left in the group. From there there's no particular variation
            // to improve any timing therefore I consider them particular cases.
            //
            // The hikers are represented by their pace and I sort them before
            // crossing each bridge. The choice of using a std::vector to hold
            // the hikers and std::sort sorting algorithm with [n*log(n)] time
            // complexity potentially may be faster than other data-structures
            // (have to prove) for a reasonable numbers of hikers in each group
            // as long as each group fits in a line of CPU cache in order to
            // avoid false sharing at sorting.

            auto& bridge = bridges[k];
            for (size_t n = groups[k].size(); n > 0; n = groups[k].size())
            {
                if (n == 1)
                {
                    // one-way trip
                    // timing for hiker [0]
                    bridge.time += bridge.length / groups[k][0];

                    // transfer for the last hiker
                    groups[k+1].push_back(groups[k][0]);
                    groups[k].pop_back();
                }
                else if (n == 2)
                {
                    // one-way trip
                    // timing for hikers [0] and [1]
                    bridge.time += bridge.length / groups[k][1];

                    // transfer for the last 2 hikers
                    groups[k+1].push_back(groups[k][1]);
                    groups[k+1].push_back(groups[k][0]);
                    groups[k].pop_back();
                    groups[k].pop_back();
                }
                else if (n == 3)
                {
                    // first round-trip
                    // timing for the fastest pair of hikers [0] and [1] ([0] crosses back)
                    bridge.time += bridge.length / groups[k][1];
                    bridge.time += bridge.length / groups[k][0];

                    // second one-way trip
                    // timing for the slowest pair of hikers [2] and [0]
                    bridge.time += bridge.length / groups[k][2];

                    // transfer for the last 3 hikers
                    groups[k+1].push_back(groups[k][2]);
                    groups[k+1].push_back(groups[k][1]);
                    groups[k+1].push_back(groups[k][0]);
                    groups[k].pop_back();
                    groups[k].pop_back();
                    groups[k].pop_back();
                }
                else
                {
                    // first round-trip
                    // timing for the fastest pair of hikers [0] and [1] ([0] crosses back)
                    bridge.time += bridge.length / groups[k][1];
                    bridge.time += bridge.length / groups[k][0];

                    // second round-trip
                    // timing for the slowest pair of hikers [j] and [j-1] ([1] crosses back)
                    size_t j = n - 1;
                    bridge.time += bridge.length / groups[k][j];
                    bridge.time += bridge.length / groups[k][1];

                    // transfer the slowest pair of hikers
                    groups[k+1].push_back(groups[k][j-1]);
                    groups[k+1].push_back(groups[k][j]);
                    groups[k].pop_back();
                    groups[k].pop_back();
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

