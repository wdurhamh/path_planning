#include "Point.h"
#include "IObstacle.h"

#pragma once

class ObstCircle: public IObstacle{
    public:
        float r;
        Point c;

        ObstCircle(const Point p, float r);

        bool containsPt(const Point &p) const override;

        bool intersectsSegment(const Point &p1, const Point &p2) const override;
};