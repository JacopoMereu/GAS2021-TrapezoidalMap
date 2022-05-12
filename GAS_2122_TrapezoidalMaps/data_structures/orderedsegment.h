#ifndef ORDEREDSEGMENT_H
#define ORDEREDSEGMENT_H

#include "cg3/geometry/segment2.h"
#include "cg3/geometry/point2.h"


// Class created because we need to know the leftmost/rightmost verteces of a segment but I don't want to check it every time a runtime...
/**
 * @extends cg3::Segment2d
 * @brief The OrderedSegment class rapresents a segment sorted by the x-value (from the endpoint with smallest x to the endpoint with highest x).
 * I recall segments are in general position, ergo the two endpoints of the segment cannot have the same x-value.
 */
class OrderedSegment : public cg3::Segment2d
{
public:
    // Trivial constructors
    OrderedSegment(const cg3::Segment2d unordered_s);
    OrderedSegment(const cg3::Point2d unordered_p1, const cg3::Point2d unordered_p2);

    /**
     * @brief getLeftmost
     * @return the endpoint of the segment with SMALLEST x-value.
     */
    const cg3::Point2d& getLeftmost() const;

    /**
     * @brief getRightmost
     * @return the endpoint of the segment with HIGHEST x-value.
     */
    const cg3::Point2d& getRightmost() const;

private:
    /**
     * @brief orderSegment sort the two endpoints if there's need.
     * At the end the inheritated attribute "p1" will contain the leftmost point,
     * while the inheritated attribute "p2" will contain the rightmost point.
     */
    void orderSegment();
};

#endif // ORDEREDSEGMENT_H
