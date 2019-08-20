#include "algo.h"
#include <algorithm>

// This Algo computes the crossing time and total distance
// The assumption is made that a faster hiker on the right side of the bridge
// can't help the hikers on the left side to improve the crossing time.
// Each group on the left side independently has to cross the bridge and
// then join the group of hikers on the right side.

void Algo::show() const
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

Algo::Algo(const Yaml& yaml)
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

void Algo::run()
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

std::ostream& operator<<(std::ostream& os, const Algo& algo)
{
    for (size_t k = 0; k < algo.bridges.size(); ++k)
        os << "bridge " << k << ": " << algo.bridges[k] << std::endl;
    os << "overall : " << algo.result;
    return os;
}

