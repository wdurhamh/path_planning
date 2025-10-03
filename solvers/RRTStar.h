#include "../env/ProblemInstance.h"
#include "../env/Point.h"
#include "RRT.h"
#include "KDTree.h"

#include<vector>
#include<memory>
#include<string>

class RRTStar{
    public:
    
    RRTStar(float rMax, float epsGoal, float minIters);

    std::vector<Point> solve(ProblemInstance pInst);

    void treeToFile(std::string fName);

    private:
    //Initialized in constructor
    float rMax;
    float epsGoal;
    float minIters;
    //cache off rMax^2, which will be used repeatedly.
    float rMaxSqr;
    bool solFound;


    //the RRT itself, accessible via the root node
    std::shared_ptr<RRTNode> root;

    std::vector<Point> extractPath(RRTNode* bestEnd);

    void growTree(ProblemInstance &pInst, KDTree &kdTree, int iters, RRTNode* endNode);
};