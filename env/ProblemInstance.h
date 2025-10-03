#include "Workspace.h"
#include "Point.h"

#pragma once

class ProblemInstance{
    public:
        Workspace &w;
        const Point &start;
        const Point &end;

        ProblemInstance(Workspace &w, const Point &start, const Point &end): w(w), start(start), end(end){}

};