#include "AbstractPolygon.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// -----------------------------------------------------------------------------
// Point on 2D plane

EC2DPoint :: EC2DPoint(int x, int y): x(x), y(y) {}
    
int EC2DPoint :: GetX() const{
    return x;
}

int EC2DPoint :: GetY() const{
    return y;
}

int EC2DPoint :: Distance(const EC2DPoint &rhs) const{
    int x2 = rhs.GetX();
    int y2 = rhs.GetY();

    return sqrt(pow((x - x2), 2) + pow((y - y2), 2));
}

EC2DPoint& EC2DPoint :: operator=(const EC2DPoint &new_point){ 
    x = new_point.GetX();
    y = new_point.GetY();
    return *this;
}

void EC2DPoint :: Print() const{
    std::cout << "point (" << x << "," << y << ")" << std::endl;
}

// -----------------------------------------------------------------------------
// Convex polygon on 2D plane: interface class
// your code here
AbstractPolygon :: AbstractPolygon() {}

std::vector<AbstractPolygon *> AbstractPolygon :: GetChildren() const { return children; }

void AbstractPolygon :: AddChild(AbstractPolygon *child) { children.push_back(child); }

EC2DPoint AbstractPolygon :: GetTopLeft() { return top_left; }
EC2DPoint AbstractPolygon ::  GetBotRight() { return bottom_right; }