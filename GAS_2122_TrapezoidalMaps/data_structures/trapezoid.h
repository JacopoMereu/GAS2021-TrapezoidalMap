#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>

class Trapezoid
{
public:
    Trapezoid(cg3::Segment2 t, cg3::Segment2 b, cg3::Point2 lp, cg3::Point2 rp);

private:
    cg3::Segment2 top;
    cg3::Segment2 bottom;
    cg3::Point2 leftp;
    cg3::Point2 rightp;


};

#endif // TRAPEZOID_H
