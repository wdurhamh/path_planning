#include "../env/Point.h"

#include<vector>
#include<memory>


#pragma once

struct RRTNode{
            std::weak_ptr<RRTNode> parent;
            std::vector<std::shared_ptr<RRTNode>> children;
            float cost_to_here;
            Point p;
};