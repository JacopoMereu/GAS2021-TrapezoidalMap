#include "trapezoid.h"



Trapezoid::Trapezoid(OrderedSegment& t, OrderedSegment& b, cg3::Point2& lp, cg3::Point2& rp) : top(t), bottom(b), leftp(lp), rightp(rp)
{

}

OrderedSegment &Trapezoid::getTop() const
{
    return top;
}

void Trapezoid::setTop(const OrderedSegment &newTop)
{
    top = newTop;
}

OrderedSegment &Trapezoid::getBottom() const
{
    return bottom;
}

void Trapezoid::setBottom(const OrderedSegment &newBottom)
{
    bottom = newBottom;
}

cg3::Point2 &Trapezoid::getLeftp() const
{
    return leftp;
}

void Trapezoid::setLeftp(const cg3::Point2 &newLeftp)
{
    leftp = newLeftp;
}

cg3::Point2 &Trapezoid::getRightp() const
{
    return rightp;
}

void Trapezoid::setRightp(const cg3::Point2 &newRightp)
{
    rightp = newRightp;
}


// setter/getter of the trapezoidal neighbors
void Trapezoid::setUpperLeftNeighbor(Trapezoid& newNeighbor) {
    neighbors[TOPLEFT] = newNeighbor;
}
void Trapezoid::setUpperRightNeighbor(Trapezoid& newNeighbor) {
    neighbors[TOPRIGHT] = newNeighbor;
}
void Trapezoid::setLowerLeftNeighbor(Trapezoid& newNeighbor) {
    neighbors[BOTTOMLEFT] = newNeighbor;
}
void Trapezoid::setLowerRightNeighbor(Trapezoid& newNeighbor) {
    neighbors[BOTTOMRIGHT] = newNeighbor;
}

//
Trapezoid& Trapezoid::getUpperLeftNeighbor() {
    return neighbors[TOPLEFT];
}
Trapezoid& Trapezoid::getUpperRightNeighbor() {
    return neighbors[TOPRIGHT];
}
Trapezoid& Trapezoid::getLowerLeftNeighbor() {
    return neighbors[BOTTOMLEFT];
}
Trapezoid& Trapezoid::getLowerRightNeighbor() {
    return neighbors[BOTTOMRIGHT];
}
