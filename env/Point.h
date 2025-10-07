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

        float sqrDistance(const Point &other){
            return pow(x - other.x, 2) + pow(y - other.y, 2);
        }

        float distance(const Point &other){
            return sqrt(sqrDistance(other));
        }

        float cross(const Point &other){
            return x*other.y - y*other.x;
        }

        Point operator -= (const Point &other){
          x -= other.x;
          y -= other.y;
          return *this;
        }

        Point operator += (const Point &other){
          x += other.x;
          y += other.y;
          return *this;
        }

};

inline bool operator == (const Point &p1, const Point &p2){
  return (p1.x==p2.x && p1.y==p2.y);
}

inline Point operator - (const Point &p1, const Point &p2){
    Point newPoint = p1;
    return newPoint -=p2;
}

inline Point operator - (const Point &p1){
  return Point(-p1.x, -p1.y);

}

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
