#include "orderedsegment.h"

OrderedSegment::OrderedSegment()
{
    if(this->p1()>this->p2()) {
        cg3::Point2 tmp = p2;
        setP1(p2);
        setP2(tmp);
    }
}


Point2& OrderedSegment::getLeftmost() {
    return this->p1();
}

Point2& OrderedSegment::getRightmost() {
    return this->p2();
}
