#ifndef TRAPEZOID_H
#define TRAPEZOID_H

//#include <cg3/geometry/segment2.h>
#include "orderedsegment.h"
#include <cg3/geometry/point2.h>

class Trapezoid
{
public:
    // A trapezoid built using general position segments has at most 4 neighbors
    const int N_NEIGHBORS = 4;

    Trapezoid(OrderedSegment& t, OrderedSegment& b, cg3::Point2& lp, cg3::Point2& rp);

    OrderedSegment &getTop() const;
    void setTop(const OrderedSegment &newTop);

    OrderedSegment &getBottom() const;
    void setBottom(const OrderedSegment &newBottom);

    cg3::Point2 &getLeftp() const;
    void setLeftp(const cg3::Point2 &newLeftp);

    cg3::Point2 &getRightp() const;
    void setRightp(const cg3::Point2 &newRightp);

    // setter/getter of the trapezoidal neighbors
    void setUpperLeftNeighbor(Trapezoid& newNeighbor);
    void setUpperRightNeighbor(Trapezoid& newNeighbor);
    void setLowerLeftNeighbor(Trapezoid& newNeighbor);
    void setLowerRightNeighbor(Trapezoid& newNeighbor);
    Trapezoid& getUpperLeftNeighbor();
    Trapezoid& getUpperRightNeighbor();
    Trapezoid& getLowerLeftNeighbor();
    Trapezoid& getLowerRightNeighbor();

private:
    OrderedSegment& top;     // reference to the top segment
    OrderedSegment& bottom;  // reference to the bottom segment
    cg3::Point2& leftp;     // reference to the the left point
    cg3::Point2& rightp;    // reference to the top right point

    enum neighborsCode {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT};
    std::array<&Trapezoid, N_NEIGHBORS> neighbors = {nullptr, nullptr, nullptr, nullptr}; // array containing the adjacent trapezoids.
};

#endif // TRAPEZOID_H
