#include "AStar.h"
#include "../env/Point.h"
#include "../env/ProblemInstance.h"

#include<vector>
#include<functional>
#include<unordered_set>
#include<utility>
#include<unordered_map>
#include<algorithm>
#include<fstream>
#include<iostream>


std::pair<int, int> AStar::neighbors[8] = {{-1,-1}, {-1,0}, {-1,1},
                                           {0,-1}, {0,1},
                                           {1,-1}, {1,0}, {1,1}};

AStar::AStar(double _gridStep, std::function<double(Point, Point)> heuristic)
        :gridStep(_gridStep), heuristic(heuristic){}

std::vector<Point> AStar::solve(ProblemInstance &pInst){
    std::unordered_map<Point, std::weak_ptr<AStarNode>> visitMap;

    root = std::make_shared<AStarNode>();
    root->p = pInst.start;
    root->costToGo = heuristic(pInst.start, pInst.end);
    root->costToHere = 0;
    visitMap[pInst.start] = root;

    priorityQueue.push(root);

    std::unordered_set<Point> expanded;
    std::shared_ptr<AStarNode> endNode;
    while (!endNode && !priorityQueue.empty()){
        std::shared_ptr<AStarNode> node = priorityQueue.top();
        priorityQueue.pop();
        if (expanded.count(node->p)){
            continue;
        }
        //marked as visited
        expanded.insert(node->p);
        for (auto neighbor : neighbors){
            double x = node->p.x + gridStep*neighbor.first;
            double y = node->p.y + gridStep*neighbor.second;
            Point p(x,y);
            if (expanded.count(p) == 0 
                && pInst.w.inWorkspace(p)
                &&!pInst.w.segmentIntersectsObstacle(p, node->p)){
                //check if this point has been added already
                float dist = p.distance(node->p);
                auto key = visitMap.find(p);
                std::shared_ptr<AStarNode> child;
                if (key != visitMap.end()){
                    //if so, check if this path is a better path
                    std::shared_ptr<AStarNode> possibleChild = key->second.lock();
                    if (possibleChild && (possibleChild->costToHere > node->costToHere + dist) ){
                        child = possibleChild;
                    }   
                }
                else{
                    //otherwise, create new AStar node and add to queue
                    std::shared_ptr<AStarNode> newNode = std::make_shared<AStarNode>();
                    newNode->p = p;
                    newNode->costToGo = heuristic(p, pInst.end);
                    child = newNode;
                    visitMap[p] = newNode;
                }

                if (child){
                    child->costToHere = node->costToHere + dist;
                    priorityQueue.push(child);//even if it's in the queue, we add again because it has a new score
                    node->addChild(child);
                    //finally, check if we made it to the end
                    if (p.distance(pInst.end) <= gridStep/std::sqrt(2)){
                        endNode = child;
                    }
                }
            }
        }
    }
    return extractPath(endNode);
}

void AStar::treeToFile(std::string fName){
    std::ofstream outFile(fName);
    std::queue<AStarNode*> toExpand;
    if (root){//make sure the root has been initialized before doing anything here
        toExpand.push(root.get());
        while (!toExpand.empty()){
            AStarNode* current = toExpand.front();
            toExpand.pop();
            for (auto child : current->children ){
                if (child){
                        outFile<<current->p.x<<","<<current->p.y<<",";
                    outFile<<child->p.x<<","<<child->p.y<<"\n";
                }
                toExpand.push(child.get());
            }
        }
    }
    outFile.close();
}


std::vector<Point> AStar::extractPath(std::shared_ptr<AStarNode> end){
    std::vector<Point> path;
    AStarNode* current = end.get();
    while (current != nullptr){
        path.push_back(current->p);
        current = current->parent.lock().get();
    }

    std::reverse(path.begin(), path.end());
    return path;
} 


bool AStar::AStarNode::operator <(const AStarNode &other) const{
    //defining for use in the queue, where we want better (smaller) scores 
    //to take priority
    return costToHere + costToGo > other.costToHere + other.costToGo;
}

void AStar::AStarNode::removeChild(std::shared_ptr<AStarNode> child){
    children.erase(child);
}

void AStar::AStarNode::addChild(std::shared_ptr<AStarNode> child){
    std::shared_ptr<AStarNode> oldParent = child->parent.lock();
    if (oldParent){
        oldParent->removeChild(child);
    }

    child->parent = shared_from_this();
    children.insert(child);
}