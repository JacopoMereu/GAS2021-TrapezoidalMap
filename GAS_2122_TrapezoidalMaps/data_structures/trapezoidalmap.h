#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "dag.h"
#include "trapezoid.h"
#include <set>
#include "cg3/geometry/bounding_box2.h"
//#include "algorithms/OrientationUtility.h"

class TrapezoidalMap
{
public:
    TrapezoidalMap();
    void initialize(const cg3::BoundingBox2& B);
    void addSegment(OrderedSegment& segment);

private:
    DAG D;
    std::vector<Trapezoid*> T;   // Contains the trapezoids
    std::vector<cg3::Point2d> points;  // Contains the points
    std::vector<OrderedSegment> segments;    // Contains the segments

    void followSegment(OrderedSegment& s, std::vector<Trapezoid*> facesIntersectingSegment);
    void split(OrderedSegment& s, std::vector<Trapezoid*> intersectingFaces);
//    void splitFaceIntersectingSegment(Trapezoid& face, OrderedSegment s, std::vector<Trapezoid>& newFaces);
};

#endif // TRAPEZOIDALMAP_H
