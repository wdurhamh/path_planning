#include "env/ProblemInstance.h"
#include "env/Workspace.h"
#include "env/Point.h"
#include "solvers/RRTStar.h"
#include "solvers/AStar.h"

#include<iostream>
#include<functional>


int main(){
    Workspace w(0, 10, 0, 10);
    w.addObstacle(Point(3,2.5), 1);

    w.addObstacle(Point(4,8), 1);
    w.addObstacle(Point(7,5), 1);
    w.addObstacle(Point(9,5), 1);

    Point start(2.0, 0.0);
    Point end(10.0, 10.0);

    ProblemInstance pInst(w, start, end);

    // float rMax = 0.5;
    // float epsGoal = 0.25;
    // float minIters = 10;
    // RRTStar solver(rMax, epsGoal, minIters);

    double gridStep = 0.25;
    std::function<double(Point, Point)> heuristic = [](Point p1, Point p2){return double(p1.distance(p2));};
    AStar solver(gridStep, heuristic);

    std::vector<Point> solution = solver.solve(pInst);
    solver.treeToFile("tree.txt");
    std::cout<<"nice"<<std::endl;

    return 0;
}