#include "KDTree.h"
#include "../env/Point.h"
#include<memory>
#include<cmath>
#include<iostream>


void KDTree::add(std::shared_ptr<RRTNode> rrtNode){
    //check if the node has been initialized
    std::shared_ptr<KDNode> node = std::make_shared<KDNode>();
    node->rrtNode = rrtNode;
    if (!root){
        root = node;
        return;
    }

    root->insert(node);
    return;
}

std::shared_ptr<RRTNode> KDTree::find_nearest(const Point &p){
    float bestDist;
    return root->find_nearest(p, bestDist);
}

std::shared_ptr<RRTNode> KDTree::KDNode::find_nearest(const Point &p, float &bestSoFar){
    std::shared_ptr<RRTNode> bestInSub;
    KDNode* next;
    KDNode* otherChild;

    std::shared_ptr<RRTNode> validRN = rrtNode.lock();
    if ( (d && p.x >= validRN->p.x) || (d==0 && p.y >= validRN->p.y)){
        next = right.get();
        otherChild = left.get();
    }
    else{
        next = left.get();
        otherChild = right.get();
    }

    float distToPoint = validRN->p.sqrDistance(p); 
    if (next == nullptr){
        //base case
        bestSoFar = distToPoint;
        bestInSub = validRN;
    }
    else{
        //continue looking at next
        bestInSub = next->find_nearest(p, bestSoFar);

        //check if this node is better
        if (distToPoint < bestSoFar){
            bestSoFar = distToPoint;
            bestInSub = validRN;
        }
        //check distance to the dividing plane
        float toPlane = this->distToDivide(p);
        if (bestSoFar > std::pow(toPlane, 2) && otherChild!=nullptr){
            //check the subtree
            float bestSoFarOther;
            std::shared_ptr<RRTNode> nearestInOther = otherChild->find_nearest(p, bestSoFarOther);
            if (bestSoFarOther < bestSoFar){
                bestSoFar = bestSoFarOther;
                bestInSub = nearestInOther;
            }
        }
    }

    return bestInSub;
}

float KDTree::KDNode::distToDivide(const Point &p){
    std::shared_ptr<RRTNode> validRN = rrtNode.lock();
    if (validRN){
        if (d){
            return std::abs(p.x - validRN->p.x);
        }
        else{
            return std::abs(p.y - validRN->p.y);
        }
    }
    else{
        //there was an error trying to access the rrtNode
        //return invalid distance to flag this, but irl this should never happen
        return -1.0;
    }
    
}


void KDTree::KDNode::insert(std::shared_ptr<KDTree::KDNode> node){
    std::shared_ptr<KDTree::KDNode> nextNode = nullptr;
    std::shared_ptr<RRTNode> newNode = node->rrtNode.lock();
    std::shared_ptr<RRTNode> thisNode = rrtNode.lock();

    if (newNode && thisNode){
        if (d){
            if (newNode->p.x >= thisNode->p.x){
                nextNode = right;
            }
            else{
                nextNode = left;
            }
        }
        else{
            if (newNode->p.y >= thisNode->p.y){
                nextNode = right;
            }
            else{
                nextNode = left;
            }
        }

        if (nextNode){
            nextNode->insert(std::move(node));
        }
        else if ( (d && newNode->p.x >= thisNode->p.x) || (d==0 && newNode->p.y >= thisNode->p.y)) {
                right = std::move(node);
                right->parent = shared_from_this();
                right->d = (d+1)%2;                    
        }
        else {
            left = std::move(node);
            left->parent = shared_from_this();
            left->d = (d+1)%2;
        }
    }
}