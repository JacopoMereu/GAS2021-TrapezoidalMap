#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "dag.h"
#include "trapezoid.h"
#include <set>
#include "cg3/geometry/bounding_box2.h"

class TrapezoidalMap
{
public:
    TrapezoidalMap(cg3::BoundingBox2& B);
    void addSegment(cg3::Segment2& segment);

private:
    DAG D;
    std::vector<Trapezoid> T;   // Contains the trapezoids
    std::vector<cg3::Point2> T;  // Contains the points
    std::vector<cg3::Segment> T;    // Contains the segments

    std::vector<Trapezoid&> getFacesIntersectingSegment(cg3::Segment s);
    void splitFaceIntersectingSegment(Trapezoid& face, cg3::Segment s, std::vector<Trapezoid>& newFaces);
};

#endif // TRAPEZOIDALMAP_H
