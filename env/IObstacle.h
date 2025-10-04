#include "Point.h"

#pragma once

class IObstacle{
public:
    virtual ~IObstacle() = default;

    virtual bool containsPt(const Point &p) const = 0;

    virtual bool intersectsSegment(const Point &p1, const Point &p2) const = 0;

};