#ifndef ORDEREDSEGMENT_H
#define ORDEREDSEGMENT_H

#include "cg3/geometry/segment2.h"
#include "cg3/geometry/point2.h"


// Class created because we need to know the leftmost/rightmost verteces of a segment but I don't want to check it every time a runtime...
class OrderedSegment : private cg3::Segment2
{
public:
    OrderedSegment();
    cg3::Point2& getLeftmost();
    cg3::Point2& getRightmost();
};

#endif // ORDEREDSEGMENT_H
