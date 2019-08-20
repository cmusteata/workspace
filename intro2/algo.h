/*******************************************************************************
 *                         Author: Constantin Musteata                         *
 *                          costin.musteata@gmail.com                          *
 ******************************************************************************/

#ifndef __ALGO_H__
#define __ALGO_H__

#include <vector>
#include <deque>
#include <ostream>
#include "bridge.h"
#include "yaml.h"

// This algo computes the crossing time and total distance
// The assumption is made that a faster hiker on the right side
// may help the hikers on the left side to improve the crossing time
// After each pair-crossing from the left side to the right side
// the ranking of the hikers is reevaluated in order to establish
// the fastest hiker to return the torch back to the left side.

class Algo final
{
    Bridge result;
    std::vector<Bridge> bridges;
    std::vector<std::deque<double>> groups;

    void show() const;
public:
    Algo(const Yaml&);
    void run();

    friend std::ostream& operator <<(std::ostream&, const Algo&);
};

#endif //__ALGO_H__

