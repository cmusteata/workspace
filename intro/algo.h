/*******************************************************************************
 *                         Author: Constantin Musteata                         *
 *                          costin.musteata@gmail.com                          *
 ******************************************************************************/

#ifndef __ALGO_H__
#define __ALGO_H__

#include <vector>
#include <ostream>
#include "bridge.h"
#include "yaml.h"

// This algo computes the crossing time and total distance
// The assumption is made that a faster hiker on the right side of the bridge
// can't help the hikers on the left side to improve the crossing time.
// Each group on the left side independently has to cross the bridge and
// then join the group of hikers on the right side.

class Algo final
{
    Bridge result;
    std::vector<Bridge> bridges;
    std::vector<std::vector<double>> groups;

    void show() const;
public:
    Algo(const Yaml&);
    void run();

    friend std::ostream& operator <<(std::ostream&, const Algo&);
};

#endif //__ALGO_H__

