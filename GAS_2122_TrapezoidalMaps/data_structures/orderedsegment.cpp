#include "orderedsegment.h"

OrderedSegment::OrderedSegment(const cg3::Segment2d unordered_s)  : cg3::Segment2d(unordered_s) {
    orderSegment();
}

OrderedSegment::OrderedSegment(const cg3::Point2d unordered_p1, const cg3::Point2d unordered_p2) : cg3::Segment2d(unordered_p1, unordered_p2)  {
    orderSegment();
}

const cg3::Point2d& OrderedSegment::getLeftmost() const {
    return (this->p1());
}

const cg3::Point2d& OrderedSegment::getRightmost() const  {
    return this->p2();
}

void OrderedSegment::orderSegment() {
    if(this->p1().x() > this->p2().x()) {
        cg3::Point2d tmp = this->p1();
        setP1(this->p2());
        setP2(tmp);
    }
}
