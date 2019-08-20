#include "bridge.h"

Bridge::Bridge()
: length(0)
, time(0.0)
{
}

Bridge::Bridge(int len)
: length(len)
, time(0.0)
{
}

const Bridge& Bridge::operator +=(const Bridge& bridge)
{
    length += bridge.length;
    time   += bridge.time;
    return *this;
}

std::ostream& operator <<(std::ostream& os, const Bridge& bridge)
{
    os << "[" << bridge.length << " ft, " << bridge.time << " min]";
    return os;
}


