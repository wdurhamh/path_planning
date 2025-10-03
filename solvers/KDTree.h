#include "RRT.h"
#include "../env/Point.h"

#include<memory>

#pragma once

class KDTree{
    private:
        class KDNode : public std::enable_shared_from_this<KDNode>{
            public:
                std::weak_ptr<KDNode> parent;
                std::shared_ptr<KDNode> left;
                std::shared_ptr<KDNode> right;
                std::weak_ptr<RRTNode> rrtNode;
                int d;//dimension we're splitting on

                void insert(std::shared_ptr<KDNode> node);
                std::shared_ptr<RRTNode> find_nearest(const Point &p, float &bestSoFar);
            private:
                float distToDivide(const Point &p);
        };
        
    public:
        std::shared_ptr<KDNode> root;

        std::shared_ptr<RRTNode> find_nearest(const Point &p);
        void add(std::shared_ptr<RRTNode> rrtNode);
};