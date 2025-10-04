#include "Workspace.h"
#include<random>

Workspace::Workspace(float x_min, float x_max, float y_min, float y_max):
            x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max),
            gen(std::random_device{}()), distx(x_min, x_max), disty(y_min, y_max){}

bool Workspace::inWorkspace(const Point &p) const{
    return (p.x <= x_max && p.x >= x_min && p.y <= y_max && p.y >= y_min);
}

bool Workspace::ptInObstacle(const Point &p) const {

    for (auto obstacle : obstacles){
        if (obstacle.containsPt(p)){
            return true;
        }
    }
    return false;
}

bool Workspace::segmentIntersectsObstacle(const Point &p1, const Point &p2) const {
    for (auto obstacle: obstacles){
        if (obstacle.intersectsSegment(p1, p2)){
            return true;
        }
    }
    return false;
}



Point Workspace::sample() {
    float x = distx(gen);
    float y = disty(gen);
    return Point(x, y);
}

void Workspace::addObstacle(const Point &p, const float r){
    Obstacle o(p, r);
    obstacles.push_back(o);
}