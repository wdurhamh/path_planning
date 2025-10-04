#include<cmath>
#include<functional>

#pragma once

class Point{
    public:
        float x;
        float y;

        //keep a default constructor
        Point(){}
        
        Point(float x, float y): x(x), y(y){}

        bool operator==(const Point &other) const {
            return (this->x == other.x && this->y == other.y);
        }

        float sqrDistance(const Point &other){
            return pow(x - other.x, 2) + pow(y - other.y, 2);
        }

        float distance(const Point &other){
            return sqrt(sqrDistance(other));
        }
};

template <>
struct std::hash<Point>
{
  std::size_t operator()(const Point& p) const
  {
    using std::hash;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return (std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1));
  }
};
