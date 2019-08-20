#include "algo.h"
#include <algorithm>

// This algo computes the crossing time and total distance
// The assumption is made that a faster hiker on the right side
// may help the hikers on the left side to improve the crossing time
// After each pair-crossing from the left side to the right side
// the ranking of the hikers is reevaluated in order to establish
// the fastest hiker to return the torch back to the left side.

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

        // The top 2 fastest hikers on the left side cross together at the
        // pace of the second fastest. After this first pair-crossing we
        // reevaluate the fastest hikers on the right. The fastest hiker on
        // the right group returns with the torch and hands it over to the
        // slowest pair of hikers on the left side. The slowest pair of hikers
        // finally crosses the bridge. After this second pair-crossing we
        // have to reevaluate again the fastest hiker on the right side.
        // The fastest hiker on the right side returns with the torch.
        // At this point the fastest hikers in both left and right side
        // are on the left side helping the rest of the group to cross.
        // 
        // We repeat this same algorithm for each bridge by removing the
        // bottom 2 slowest hikers at each iteration until 1, 2 or 3 hikers
        // are left in the group. From there, there's no particular variation
        // to improve any timing therefore I consider them particular cases.
        //
        // The hikers are represented by their pace and we sort them after
        // each bridge crossing. I use std::deque to move the hikers back and
        // forth and std::sort algorithm with [n*log(n)] time complexity.

        auto &bridge = bridges[k];
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

                // move the fastest hiker from left to the right side
                // and reevaluate the fastest hiker on the right side
                rhs.push_front(lhs.front());
                lhs.pop_front();
                std::sort(rhs.begin(), rhs.end(), std::greater<double>());
                show();

                // move the fastest hiker from right to the left side
                // and reevaluate the fastest hiker on the left side
                lhs.push_front(rhs.front());
                rhs.pop_front();
                std::sort(lhs.begin(), lhs.end(), std::greater<double>());
                show();

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

                // NOTE: in real life the fastest hikers don't return the
                // torch together as in this code. They return the torch
                // at different points in time. This is just a code
                // optimization since after the first pair-crossing and
                // ranking reevaluation the fastest pair of hikers in both
                // groups is not going to change and they certainly
                // return afer the first 2 pair-crossings.

                // move pair of fastest hikers from left to the right side
                // and reevaluate the fastest hikers on the right side.
                rhs.push_front(lhs.front());
                lhs.pop_front();
                rhs.push_front(lhs.front());
                lhs.pop_front();
                std::sort(rhs.begin(), rhs.end(), std::greater<double>());
                show();

                // move pair of fastest hikers from right to the left side
                // and reevaluate the fastest hikers on the left side
                lhs.push_front(rhs.front());
                rhs.pop_front();
                lhs.push_front(rhs.front());
                rhs.pop_front();
                std::sort(lhs.begin(), lhs.end(), std::greater<double>());
                show();

                // timing for the fastest hiker from the right side
                // who just returned the torch from the right side
                bridge.time += bridge.length / lhs[0];

                // timing for the slowest pair of hikers from the
                // left side lhs[j] and lhs[j-1] at the pace of lhs[j]
                size_t j = n - 1;
                bridge.time += bridge.length / lhs[j];
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

