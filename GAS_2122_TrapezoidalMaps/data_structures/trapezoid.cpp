#include "trapezoid.h"



Trapezoid::Trapezoid(cg3::Segment2& t, cg3::Segment2& b, cg3::Point2& lp, cg3::Point2& rp) : top(t), bottom(b), leftp(lp), rightp(rp)
{

}

cg3::Segment2 &Trapezoid::getTop() const
{
    return top;
}

void Trapezoid::setTop(const cg3::Segment2 &newTop)
{
    top = newTop;
}

cg3::Segment2 &Trapezoid::getBottom() const
{
    return bottom;
}

void Trapezoid::setBottom(const cg3::Segment2 &newBottom)
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
