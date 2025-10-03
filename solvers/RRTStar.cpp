#include "RRTStar.h"
#include "KDTree.h"
#include "RRT.h"
#include "../env/ProblemInstance.h"
#include "../env/Point.h"

#include<vector>
#include<iostream>
#include<fstream>
#include<memory>
#include<string>
#include<queue>

      
RRTStar::RRTStar(float rMax, float epsGoal, float minIters) : rMax(rMax), epsGoal (epsGoal), minIters(minIters){
    rMaxSqr = pow(rMax, 2);
    solFound = false;
}


std::vector<Point> RRTStar::solve(ProblemInstance pInst){
    //initialize the K-D tree for storage

    //to do:
    root.reset(new RRTNode());
    root->p = pInst.start;
    root->cost_to_here = 0;

    KDTree kdTree;
    kdTree.add(root);

    bool stop = false;
    int iters = 0;
    RRTNode* endNode = nullptr;
    while (!solFound || iters < minIters){
        growTree(pInst, kdTree, iters, endNode);
        iters++;
    }
    std::cout<<"Completed in "<<iters<<" iterations."<<std::endl;

    return extractPath(endNode);
}

void RRTStar::treeToFile(std::string fName){
    std::ofstream outFile(fName);
    std::queue<RRTNode*> toExpand;
    if (root){//make sure the root has been initialized before doing anything here
        toExpand.push(root.get());
        while (!toExpand.empty()){
            RRTNode* current = toExpand.front();
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

//Private functions
std::vector<Point> RRTStar::extractPath(RRTNode* bestEnd){
    RRTNode* currentNode = bestEnd;
    std::vector<Point> path;
    while (currentNode != nullptr){
        path.push_back(currentNode->p);
        currentNode = currentNode->parent.lock().get();//very sketch, do not push
    }

    return path;
}

void RRTStar::growTree(ProblemInstance &pInst, KDTree &kdTree, int iter, RRTNode* endNode){
    Point newPoint = pInst.w.sample();//with some frequency, sample thet raget
    if (iter%10==0){
        std::cout<<"Trying goal"<<std::endl;
        newPoint = pInst.end;
    }
    std::cout<<"sampled: "<<newPoint.x<<", "<<newPoint.y<<std::endl;
    //find closest point
    std::shared_ptr<RRTNode> closest = kdTree.find_nearest(newPoint);
    std::cout<<"closest in tree: "<<closest->p.x<<", "<<closest->p.y<<std::endl;
    //Check if the point is close enough
    float sqDist =  newPoint.sqrDistance(closest->p);
    if(rMaxSqr < sqDist){
        //bring within max radius
        float theta = std::atan2(newPoint.y - closest->p.y, newPoint.x - closest->p.x);
        newPoint = Point(closest->p.x+rMax*std::cos(theta), closest->p.y+rMax*std::sin(theta));
        sqDist = rMaxSqr;
    }

    //verify path from closest to point does not go through an obstacle
    if (!pInst.w.segmentIntersectsObstacle(closest->p, newPoint)){
        //add to tree
        std::shared_ptr<RRTNode> newNode = std::make_shared<RRTNode>();
        newNode->p = newPoint;
        newNode->cost_to_here = closest->cost_to_here + sqrt(sqDist);
        newNode->parent = closest;
        closest->children.push_back(newNode);

        kdTree.add(newNode);

        std::cout<<"adding point: "<<newPoint.x<<", "<<newPoint.y<<std::endl;


        //if we're doing rrt*, do some rewiring
    
        //check if goal point has been reached
        std::cout<<newPoint.distance(pInst.end)<<std::endl;
        if (newPoint.distance(pInst.end) < epsGoal){
            solFound = true;
            //this will need to be revisited once we
            //implement re-wiring
            std::cout<<"setting the best node..."<<std::endl;
            if (endNode == nullptr){
                endNode = newNode.get();
            }
            else if (endNode->cost_to_here > newNode->cost_to_here){
                endNode = newNode.get();
                std::cout<<"got the best end node"<<std::endl;
            }
        }
    }
    
}