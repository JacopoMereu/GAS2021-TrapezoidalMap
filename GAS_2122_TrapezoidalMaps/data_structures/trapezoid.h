#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>

class Trapezoid
{
public:
    // A trapezoid built using general position segments has at most 4 neighbors
    const int N_NEIGHBORS = 4;

    Trapezoid(cg3::Segment2& t, cg3::Segment2& b, cg3::Point2& lp, cg3::Point2& rp);

    cg3::Segment2 &getTop() const;
    void setTop(const cg3::Segment2 &newTop);

    cg3::Segment2 &getBottom() const;
    void setBottom(const cg3::Segment2 &newBottom);

    cg3::Point2 &getLeftp() const;
    void setLeftp(const cg3::Point2 &newLeftp);

    cg3::Point2 &getRightp() const;
    void setRightp(const cg3::Point2 &newRightp);

private:
    cg3::Segment2& top;     // reference to the top segment
    cg3::Segment2& bottom;  // reference to the bottom segment
    cg3::Point2& leftp;     // reference to the the left point
    cg3::Point2& rightp;    // reference to the top right point

    std::array<&Trapezoid, N_NEIGHBORS> neighbors = {nullptr, nullptr, nullptr, nullptr}; // array containing the adjacent trapezoids.
};

#endif // TRAPEZOID_H
