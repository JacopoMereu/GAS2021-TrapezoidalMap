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


    void followSegment(OrderedSegment& s, std::vector<DrawableTrapezoid*>& facesIntersectingSegment);
    void split(OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);
    void splitSingularTrapezoid(OrderedSegment& s, DrawableTrapezoid* faceToSplit);
    void splitMultipleTrapezoid(OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);
    void addTrapezoidToMap(DrawableTrapezoid* trapezoidToAdd);
};

#endif // TRAPEZOIDALMAP_H
