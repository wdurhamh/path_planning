#include<vector>
#include<random>
#include<chrono>
#include<iostream>
#include<memory>
#include "KDTree.h"
#include "RRT.h"
#include "../env/Point.h"


int main(){

    //add a bunch of random points to a KD tree
    int n = 10000;
    std::vector<std::shared_ptr<RRTNode>> nodes(n);
    float xmin = -10;
    float xmax = 10;
    float ymin = -10;
    float ymax = 10;
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> distx(xmin, xmax);
    std::uniform_real_distribution<> disty(ymin, ymax);

    KDTree tree;
    //Generating random points
    std::cout<<"Generating random points ...";
    for (int i = 0; i < n; i++){
        Point p = Point(distx(gen), disty(gen));
        std::shared_ptr<RRTNode> newNode = std::make_shared<RRTNode>();
        newNode->p = p;
        nodes[i] = newNode;
        tree.add(newNode);
    }
    std::cout<<"   Done!"<<std::endl;

    int m = 1000;
    std::cout<<"Generating queries ...";
    std::vector<Point> queries(m);
    for (int j = 0; j<m; j++){
        queries.push_back(Point(distx(gen), disty(gen)));
    }
    std::cout<<"   Done!"<<std::endl;

    std::vector<Point> response(m);
    auto start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j<m; j++){
        std::shared_ptr<RRTNode> nearest = tree.find_nearest(queries[j]);
        response[j] = nearest->p;
    }
    auto elapsed =  std::chrono::high_resolution_clock::now() - start;
    std::cout<<"Time with kD tree: "
             <<std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()
             <<std::endl;

    std::vector<Point> responseLU(m);
    auto startLU = std::chrono::high_resolution_clock::now();
    for (int j = 0; j<m; j++){
        float best = 1000000;
        Point nearest;
        for (auto node : nodes){
            if (node->p.sqrDistance(queries[j]) < best){
                nearest = node->p;
                best = node->p.sqrDistance(queries[j]);
            }
        }
        responseLU[j] = nearest;
    }

    auto elapsedLU =  std::chrono::high_resolution_clock::now() - startLU;
    std::cout<<"Time with brute force: "
             <<std::chrono::duration_cast<std::chrono::milliseconds>(elapsedLU).count()
             <<std::endl;

    //finally, verify that the KD tree found the right ones
    int mismatchCount = 0;
    for (int i = 0; i< m; i++){
        if (!(response[i] == responseLU[i])){
            mismatchCount++;
        }
    }
    std::cout<<"Errors: "<<mismatchCount<<std::endl;

    return 0;
}