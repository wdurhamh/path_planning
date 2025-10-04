#include "IObstacle.h"
#include "Point.h"

#include<vector>

class ObstCvxPoly : public IObstacle {

public:

    ObstCvxPoly(std::vector<Point> vertices);

    bool containsPt(const Point &p) const override;

    bool intersectsSegment(const Point &p1, const Point &p2) const override;

private:
    //vertices must be in clockwise order
    std::vector<Point> vertices;

    bool intersectsEdge(const Point &p1, const Point &p2, const Point &p3, const Point &p4) const;
    bool inEdgeRange(const Point &p1, const Point &p2, const Point &p3) const;
};