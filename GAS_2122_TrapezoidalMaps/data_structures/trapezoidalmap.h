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
    DrawableTrapezoid* pointLocation(const cg3::Point2d& pointToQuery);
    void reset();
    void clear();
    std::vector<OrderedSegment*> segments;    // Contains the segments

protected:
    std::vector<DrawableTrapezoid*> T;   // Contains the trapezoids
    const cg3::BoundingBox2 &getBoundingBox() const;
    void setBoundingBox(const cg3::BoundingBox2 &newB);
private:
    DAG D;
    //std::vector<cg3::Point2d> points;  // Contains the points
    cg3::BoundingBox2 B;

    // Follow
    void followSegment(OrderedSegment& s, std::vector<DrawableTrapezoid*>& facesIntersectingSegment);
    // Split
    void split(OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);
    void splitSingularTrapezoid(OrderedSegment& s, DrawableTrapezoid* faceToSplit);
    void splitMultipleTrapezoid(OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);
    void stepMerging(size_t start, size_t end, std::vector<DrawableTrapezoid*>& list);
    // Add
    void addTrapezoidToMap(DrawableTrapezoid* trapezoidToAdd);
    void deleteTrapezoidFromMap(DrawableTrapezoid* trapezoidToDelete);
};

#endif // TRAPEZOIDALMAP_H
