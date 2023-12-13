#ifndef Polygon_h
#define Polygon_h

#include <vector>
#include <set>
#include "AbstractPolygon.h"
#include <cmath>

class Polygon : public AbstractPolygon {
public:
                    Polygon(
                        const EC2DPoint p1, 
                        const EC2DPoint p2,
                        const int       typein,
                        const ECGVColor colorin, 
                        const bool      filledin);
    virtual bool    IsPointInside(const EC2DPoint &s) const;
    virtual void    MoveBy(int xmove, int ymove);
    virtual void    MoveTo(EC2DPoint top_left, EC2DPoint bottom_right);
    virtual void    Print() const;
    virtual ECGVColor
                    GetColor() const { return color; }
    ECGVColor       color; 
    int             type;
    bool            filled;
};

class CompositePolygon : public AbstractPolygon {
public:
                    CompositePolygon(
                        const std::set<AbstractPolygon *> &childrenin);
    virtual bool    IsPointInside(const EC2DPoint &s) const;
    virtual void    MoveBy(int xmove, int ymove);
    virtual void    MoveTo(EC2DPoint top_left, EC2DPoint bottom_right);
    virtual void    Print() const;
};

/****
// -----------------------------------------------------------------------------
// Convex polygon on 2D plane: implementing polygon for non-triangles

class Polygon : public AbstractPolygon
{
public:
    Polygon() {}
    // Consructor takes a list of nodes, which are the nodes of the polygon
    // ordered sequentially (either clockwise or counter clockwise
    Polygon(const std::vector<EC2DPoint> &listNodes);
    
    // Test if the polygon is convex? Return false if not
    virtual bool IsConvex() const;
    
    // Calculate total area of the polygon (you can assume it is indeed convex)
    virtual double GetArea() const;
    
    // your code here if needed

    virtual void MoveBy(int xmove, int ymove);

    virtual bool IsPointInside(const EC2DPoint &s) const;

    virtual bool IsContaining(const Polygon &t) const;

    virtual bool IsIntersecting(const Polygon &t) const;

};

*******/


#endif /* Polygon_h */
