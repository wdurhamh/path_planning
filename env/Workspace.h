#include "IObstacle.h"
#include "Point.h"
#include<vector>
#include<random>
#include<memory>

#pragma once

class Workspace{
    private:
        std::mt19937 gen;
        std::uniform_real_distribution<> distx;
        std::uniform_real_distribution<> disty;

    public:
        float x_min;
        float x_max;
        float y_min;
        float y_max;

        std::vector<std::shared_ptr<IObstacle>> obstacles;

        Workspace(float x_min, float x_max, float y_min, float y_max);

        bool inWorkspace(const Point &p) const;

        bool ptInObstacle(const Point &p) const;

        bool segmentIntersectsObstacle(const Point &p1, const Point &p2) const;

        Point sample();

        void addCircObstacle(const Point &p, const float r);

        void addCvxPolyObstacle(const std::vector<Point> &vertices);
};