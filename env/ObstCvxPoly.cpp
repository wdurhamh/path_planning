#include "Point.h"
#include "ObstCvxPoly.h"

#include<vector> 
#include<cmath>




ObstCvxPoly::ObstCvxPoly(std::vector<Point> vertices):vertices(vertices){}

bool ObstCvxPoly::containsPt(const Point &p) const{
    int n = vertices.size();
    for (int i = 0; i< n; i++){
        //ensure that all cross products are less than 0
        Point a = vertices[i];
        Point b = vertices[(i+1)%n];
        
        if ((b-a).cross(p-a) >= 0){//consider a point on the boundary as being in 
            return false;
        }

    }
    return true;
}

bool ObstCvxPoly::intersectsSegment(const Point &p1, const Point &p2) const{
    int n = vertices.size();
    if (containsPt(p1) || containsPt(p2)){
        return true;
    }
    for (int i =0; i<n; i++){
        //for each line segment, check if they cross
        if (intersectsEdge(vertices[i], vertices[(i+1)%n], p1, p2)){
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////
//private functions
/////////////////////////////////////////////
bool ObstCvxPoly::intersectsEdge(const Point &p1, const Point &p2, const Point &p3, const Point &p4) const{
    double d1 = (p2 - p1).cross(p3 - p1);
    double d2 = (p2 - p1).cross(p4 - p1);
    double d3 = (p4 - p3).cross(p1 - p3);
    double d4 = (p4 - p3).cross(p2 - p3);

    if (d1*d2< 0 && d3*d4 <0){
        return true;
    }

    if ( (d1 == 0 && inEdgeRange(p1, p2, p3))
      || (d2 == 0 && inEdgeRange(p1, p2, p4))
      || (d3 == 0 && inEdgeRange(p3, p4, p1))
      || (d4 == 0 && inEdgeRange(p3, p4, p2))){
        return true;
      }
    return false;
}

bool ObstCvxPoly::inEdgeRange(const Point &p1, const Point &p2, const Point &p3) const{
    return p3.x<=std::max(p1.x, p2.x)
        && p3.x>=std::min(p1.x, p2.x)
        && p3.y<=std::max(p1.y, p2.y)
        && p3.y>=std::min(p1.y, p2.y);
}

