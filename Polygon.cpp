#include "Polygon.h"

Polygon :: Polygon(const EC2DPoint p1, const EC2DPoint p2, const int typein, const ECGVColor colorin, const bool filledin) :
   type(typein),
   filled(filledin)
{
    top_left = p1;
    bottom_right = p2;
    color = colorin;
}

//signs might be wrong/flipped for allegro for y
bool Polygon :: IsPointInside(const EC2DPoint &s) const {
    switch ( type ) {
        case 0:
            return (s.GetX() >= top_left.GetX()
                && s.GetX() <= bottom_right.GetX()
                && s.GetY() >= top_left.GetY()
                && s.GetY() <= bottom_right.GetY()
                );
        case 1:
            double x = s.GetX();
            double y = s.GetY();
            double x1 = static_cast<double>(top_left.GetX());
            double y1 = static_cast<double>(top_left.GetY());
            double x2 = static_cast<double>(bottom_right.GetX());
            double y2 = static_cast<double>(bottom_right.GetY());        
            double centerx = (x1 + x2)/2;
            double centery = (y1 + y2)/2;
            double radiusx = abs(centerx - x1);
            double radiusy = abs(centery - y1);
            double left_half = pow((x - centerx), 2) / pow(radiusx, 2);
            double right_half = pow((y - centery), 2) / pow(radiusy, 2);
            double sum = left_half + right_half;
            return sum <= 1.0;
    }
    return false;
}

void Polygon :: MoveBy(int xmove, int ymove) {
    top_left = EC2DPoint(top_left.GetX() + xmove, top_left.GetY() + ymove);
    bottom_right = EC2DPoint(bottom_right.GetX() + xmove, bottom_right.GetY() + ymove);
}

void Polygon :: MoveTo(EC2DPoint top_left_in, EC2DPoint bottom_right_in) {
    top_left = top_left_in;
    bottom_right = bottom_right_in;
}

void Polygon :: Print() const {
    std::cout << "Polygon with top left " << std::endl;
    top_left.Print();
    std::cout << "and bottom right " << std::endl;
    bottom_right.Print();
}


CompositePolygon :: CompositePolygon(const std::set<AbstractPolygon *> &childrenin)
{
    int tlx_min = 1000000;
    int tly_min = 1000000;
    int brx_max = -1000000;
    int bry_max = -1000000;
    for ( auto poly : childrenin ) {
        AddChild(poly);
        EC2DPoint tl = poly->GetTopLeft();
        EC2DPoint br = poly->GetBotRight();

        int tlx = tl.GetX();
        int tly = tl.GetY();
        int brx = br.GetX();
        int bry = br.GetY();

        tlx_min = tlx_min > tlx ? tlx : tlx_min;
        tly_min = tly_min > tly ? tly : tly_min;

        brx_max = brx_max > brx ? brx : brx_max;
        bry_max = bry_max > bry ? bry : bry_max;
    }
    top_left = EC2DPoint(tlx_min, tly_min);
    bottom_right = EC2DPoint(brx_max, bry_max);
}

bool CompositePolygon :: IsPointInside(const EC2DPoint &s) const {

    for ( auto polygon : GetChildren() ) {
        if ( polygon->IsPointInside(s) ) {
            return true;
        }
    }
    return false;
}

void CompositePolygon :: MoveBy(int xmove, int ymove) {
    for ( auto polygon : GetChildren() ) {
        polygon->MoveBy(xmove, ymove);
    }
}

void CompositePolygon :: MoveTo(EC2DPoint top_left, EC2DPoint bottom_right) {
    std::cout << "this function shouldn't be called for composites ever!" << std::endl;
}

void CompositePolygon :: Print() const {
    for ( auto polygon : GetChildren() ) {
        polygon->Print();
    }
}


/***
// -----------------------------------------------------------------------------
// Convex polygon on 2D plane: implementing polygon for non-triangles
// your code here
Polygon :: Polygon(const std::vector<EC2DPoint> &listNodes) {
    for (auto x : listNodes){
        AddNode(x);
    }
}

void Polygon :: MoveBy(int xmove, int ymove) {
    for (auto point : GetNodes()) {
        double x_new = point.GetX() + static_cast<int>(xmove);
        double y_new = point.GetY() + static_cast<int>(ymove);
        point = EC2DPoint(x_new, y_new);
    }
}
    
// Test if the polygon is convex? Return false if not
bool Polygon :: IsConvex() const{
     std::vector<EC2DPoint> nodes = GetNodes();
     std::vector<ECLineSegment> line_segs;

     // Add all line segments to line segment vector
     for(unsigned int i = 0; i < nodes.size()-1; ++i){
        ECLineSegment l(nodes[i], nodes[i+1]);
        line_segs.push_back(l);
     }
    ECLineSegment l1(nodes[nodes.size()-1], nodes[0]);
    line_segs.push_back(l1);

    bool clockwise = line_segs[0].ClockwiseAtEndpoint(line_segs[1]);
    for(unsigned int i = 1; i < line_segs.size()-1; ++i){
        if(line_segs[i].ClockwiseAtEndpoint(line_segs[i+1]) != clockwise){
            return false;
        }
    }
    return true;
}
    
// Calculate total area of the polygon (you can assume it is indeed convex)
double Polygon :: GetArea() const{
    double area = 0.0;
    
    if(GetNodes().empty()){
        return area;
    }

    std::vector<EC2DPoint> v;
    for(auto node: GetNodes()){
        bool contains = false;
        for(auto point : v){
            if(point.GetX() == node.GetX() && point.GetY() == node.GetY()){
                contains = true;
            }
        }
        if(contains == false){
            v.push_back(node);
        }
    }

    for(auto x : v){
        x.Print();
    }
    for(size_t i = 0; i < NodeCount(); ++i){
        area += v[i].GetY()*(v[i-1].GetX() - v[i+1].GetX());
    }
    return abs(area/2);
}

// Because we know that it is convex, we can just use the same method as triangle
bool Polygon :: IsPointInside(const EC2DPoint &s) const{
    std::vector<EC2DPoint> nodes = GetNodes();
    std::vector<ECLineSegment> line_segs;

    // Add all line segments to line segment vector
    for(unsigned int i = 0; i < nodes.size()-1; ++i){
        ECLineSegment l(nodes[i], nodes[i+1]);
        line_segs.push_back(l);
    }
    ECLineSegment l1(nodes[nodes.size()-1], nodes[0]);
    line_segs.push_back(l1);

    double d_test = l1.Direction(s, nodes[0], nodes[1]);
    for(unsigned int i = 1; i < line_segs.size()-1; ++i){
        double d = l1.Direction(s, nodes[i], nodes[i+1]);
        if(d_test >= 0 && d < 0 || d_test < 0 && d >= 0){
            return false;
        }
    }
    double dl = l1.Direction(s, nodes[nodes.size()-1], nodes[0]);
    if(d_test >= 0 && dl < 0 || d_test < 0 && dl >= 0){
            return false;
    }
    return true;
}

bool Polygon :: IsContaining(const Polygon &t) const{
    for(auto x : t.GetNodes()){
        if(!IsPointInside(x)){
            return false;
        }
    }
    return true;
}

bool Polygon :: IsIntersecting(const Polygon &t) const{
    if(IsContaining(t) || t.IsContaining(*this)){
        return true;
    }
    
    for(auto x : t.GetNodes()){
        if(IsPointInside(x)){
            return true;
        }
    }

    std::vector<ECLineSegment> line_segs_a;
    std::vector<EC2DPoint> v = GetNodes();

    for(unsigned int i = 0; i < v.size()-1; ++i){
        ECLineSegment l(v[i], v[i+1]);
        line_segs_a.push_back(l);
    }

    std::vector<ECLineSegment> line_segs_b;
    std::vector<EC2DPoint> vt = t.GetNodes();

    for(unsigned int i = 0; i < vt.size()-1; ++i){
        ECLineSegment l(vt[i], vt[i+1]);
        line_segs_b.push_back(l);
    }

    for(auto lsa : line_segs_a){
        for(auto lsb : line_segs_b){
            if(lsa.IsIntersect(lsb)){
                return true;
            }
        }
    }

    return false;
}

*****/