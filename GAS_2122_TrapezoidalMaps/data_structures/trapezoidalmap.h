#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "dag.h"
#include "drawables/drawabletrapezoid.h"
#include <set>
#include "cg3/geometry/bounding_box2.h"
//#include "algorithms/OrientationUtility.h"

class TrapezoidalMap
{
public:
    TrapezoidalMap();
    ~TrapezoidalMap();
    virtual void initialize(const cg3::BoundingBox2& B);
    void addSegment(const cg3::Segment2d& segment);
    DrawableTrapezoid* pointLocation(const cg3::Point2d& pointToQuery) const;
    void reset();
    void clear();

protected:
    std::vector<DrawableTrapezoid*> T;   // Contains the trapezoids
    const cg3::BoundingBox2 &getBoundingBox() const;
    void setBoundingBox(const cg3::BoundingBox2 &newB);

private:
    std::vector<OrderedSegment*> segments;    // Contains the segments
    DAG D;
    cg3::BoundingBox2 B;

    // Follow
    void followSegment(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& facesIntersectingSegment) const;
    // ///////////////////// Split//////////////////////////
    void split(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);
    void splitSingularTrapezoid(const OrderedSegment& s, DrawableTrapezoid* faceToSplit);
    void splitMultipleTrapezoid(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);
    void stepMerging(size_t start, size_t end, std::vector<DrawableTrapezoid*>& list);
    // ////////////////////////////////////////////////////

    // Add
    void addTrapezoidToMap(DrawableTrapezoid* trapezoidToAdd);
    // Delete
    void deleteTrapezoidFromMap(DrawableTrapezoid* trapezoidToDelete);
};

#endif // TRAPEZOIDALMAP_H
