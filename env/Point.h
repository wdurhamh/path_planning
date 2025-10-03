#include<cmath>

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
