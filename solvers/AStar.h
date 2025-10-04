#include "../env/Point.h"
#include "../env/ProblemInstance.h"

#include<vector>
#include<unordered_set>
#include<functional>
#include<memory>
#include<queue>
#include<utility>
#include<string>

#pragma once

class AStar{
    public:

    AStar(double gridStep, std::function<double(Point, Point)> heuristic);

    std::vector<Point> solve(ProblemInstance &pInst);

    void treeToFile(std::string fName);


    private:
    
    class AStarNode: public std::enable_shared_from_this<AStarNode>{
        public:
        std::weak_ptr<AStarNode> parent;
        std::unordered_set<std::shared_ptr<AStarNode>> children;
        Point p;
        double costToHere;
        double costToGo;

        bool operator <(const AStarNode &other) const;

        void removeChild(std::shared_ptr<AStarNode> child);
        void addChild(std::shared_ptr<AStarNode> child);
    };

    double gridStep;
    std::function<double(Point, Point)> heuristic;
    struct DerefCompare {
        bool operator()(const std::shared_ptr<AStarNode>& a, const std::shared_ptr<AStarNode>& b) const {
            return *a < *b;  // compare the objects, not the pointers
        }
    };
    std::priority_queue<std::shared_ptr<AStarNode>, std::vector<std::shared_ptr<AStarNode>>, DerefCompare> priorityQueue;
    std::shared_ptr<AStarNode> root;
    static std::pair<int, int> neighbors[8];

    std::vector<Point> extractPath(std::shared_ptr<AStarNode> node);

};