//for the sake of this practice, assume all obstacles are nice circles
#include "ObstCircle.h"
#include "Point.h"

#include<cmath>
#include<algorithm>
#include<iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

ObstCircle::ObstCircle(Point p, float r) : c(p), r(r) {}

bool ObstCircle::containsPt(const Point &p) const {
    return pow((c.x - p.x),2) + pow((c.y - p.y), 2) < pow(r,2);
}

bool ObstCircle::intersectsSegment(const Point &p1, const Point &p2) const {
    Point p2_reg = {p2.x - p1.x, p2.y - p1.y};
    float mag1Sq = pow(p2_reg.x, 2) + pow(p2_reg.y, 2);
    // std::cout<<mag1Sq<<std::endl;
    Point c_reg = {c.x - p1.x, c.y - p1.y};
    float t = (p2_reg.x*c_reg.x + p2_reg.y*c_reg.y)/mag1Sq;
    // std::cout<<t<<std::endl;
    t = std::max(t, 0.0f);
    t = std::min(t, 1.0f);
    // std::cout<<t<<std::endl;
    Point closest = {p1.x + t*p2_reg.x, p1.y + t*p2_reg.y};
    return closest.distance(c) < r;
}