#ifndef AbstractPolygon_h
#define AbstractPolygon_h

#include <vector>
#include <set>
#include <iostream>
#include "ECGraphicViewImp.h"

// -----------------------------------------------------------------------------
// Point on 2D plane

class EC2DPoint
{
public:
    EC2DPoint() {}
    virtual ~EC2DPoint() {}
    EC2DPoint(int x, int y);
    
    int GetX() const;
    int GetY() const;

    EC2DPoint& operator=(const EC2DPoint &rhs);

    int Distance(const EC2DPoint &rhs) const;

    virtual void Print() const;
private:
    int x;
    int y;
};

// -----------------------------------------------------------------------------
// Convex polygon on 2D plane: interface class
// Note: we assume there is at least three nodes in the polygon (i.e., you don't
// need to deal with invalid inputs with fewer than three nodes)

class AbstractPolygon
{
public:
    // Constructor for empty polygon
                    AbstractPolygon();
    virtual void    MoveBy(int xmove, int ymove) = 0;
    virtual void    MoveTo(EC2DPoint top_left, EC2DPoint bottom_right) = 0;

    // Test if a point is inside the polygon (if on the edge, consider it is inside)
    virtual bool    IsPointInside(const EC2DPoint &pt) const = 0;
    
    // add node (should add sequentially)
    virtual void    AddNode(const EC2DPoint &node) { listNodes.push_back(node); }
    
    virtual std::vector<EC2DPoint> 
                    GetNodes() const { return listNodes; }
    virtual  std::vector<AbstractPolygon *>
                    GetChildren() const;
    virtual EC2DPoint
                    GetTopLeft();
    virtual EC2DPoint
                    GetBotRight();
    virtual void    AddChild(AbstractPolygon *child);
    virtual void    Print() const = 0;
    virtual ECGVColor
                    GetColor() const { return ECGV_PURPLE; }
    int             type;
    bool            filled;
protected:
    EC2DPoint       top_left;
    EC2DPoint       bottom_right;
private:
    std::vector<EC2DPoint> 
                    listNodes;
    std::vector<AbstractPolygon *> 
                    children;
};

#endif /* AbstractPolygon_h */
