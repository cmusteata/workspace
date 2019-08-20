/*******************************************************************************
 *                         Author: Constantin Musteata                         *
 *                          costin.musteata@gmail.com                          *
 ******************************************************************************/

#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include <ostream>

// Data structure used internally by the 'class Algo'
// to compute the crossing distance & the crosing time

struct Bridge final
{
    int length;
    double time;
 
    Bridge();
    Bridge(int len);

    const Bridge& operator +=(const Bridge&);

    friend std::ostream& operator <<(std::ostream&, const Bridge&);
};

#endif //__BRIDGE_H__

