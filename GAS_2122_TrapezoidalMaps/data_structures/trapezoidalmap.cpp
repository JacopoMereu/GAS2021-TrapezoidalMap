#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(cg3::BoundingBox2& B)
{
    // Create a trapezoid from the bounding box
    cg3::Segment2 top = cg3::Segment(B.max()-B.lengthX(), B.max());
    cg3::Segment2 bottom = cg3::Segment(B.min(), B.min()+B.lengthX());
    cg3::Point2 leftp = cg3::Point2(B.min());
    cg3::Point2 rightp  = cg3::Point2(B.min());


    // Initialize the trapezoidal map structure T and search structure D
    Trapezoid boundingbox_trapezoid = Trapezoid(top, bottom, leftp, rightp);
    Map = std::set<Trapezoid>(boundingbox_trapezoid);
    D = DAG(&boundingbox_trapezoid);
}
