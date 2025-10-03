#include "env/ProblemInstance.h"
#include "env/Workspace.h"
#include "env/Point.h"
#include "solvers/RRTStar.h"

#include<iostream>


int main(){
    Workspace w(0, 10, 0, 10);
    w.addObstacle(Point(3,3), 1);

    Point start(0.0, 0.0);
    Point end(10.0, 10.0);

    ProblemInstance pInst(w, start, end);

    float rMax = 0.5;
    float epsGoal = 0.25;
    float minIters = 10;
    RRTStar solver(rMax, epsGoal, minIters);

    std::vector<Point> solution = solver.solve(pInst);
    solver.treeToFile("tree.txt");
    std::cout<<"nice"<<std::endl;

    return 0;
}