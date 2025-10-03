#include "Point.h"

#pragma once

class Obstacle{
    public:
        float r;
        Point c;

        Obstacle(const Point p, float r);

        bool containsPt(const Point &p);

        bool intersectsSegment(const Point &p1, const Point &p2);
};