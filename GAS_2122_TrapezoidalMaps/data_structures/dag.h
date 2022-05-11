#ifndef DAG_H
#define DAG_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "dagnode.h"

class DAG
{
public:
    // Constructor & Destructor
    DAG();
    ~DAG();

    /**
     * @brief initialize the DAG using a trapezoid
     * @param B
     */
    void initialize(DrawableTrapezoid* const B);
    void replaceNodeWithSubtree(DAGNode* const nodeToReplace, const OrderedSegment& segmentSplitting,
                                DrawableTrapezoid* const left, DrawableTrapezoid* const top,
                                DrawableTrapezoid* const bottom, DrawableTrapezoid* const right);
    DrawableTrapezoid* queryFaceContaininingPoint(const cg3::Point2d& q) const;
    DrawableTrapezoid* queryLeftmostFaceIntersectingSegment(const OrderedSegment& s) const;


private:
    // root
    DAGNode* root = nullptr;
    std::vector<DAGNode*> uniquePointers;
    DAGNode* generateNode(const cg3::Point2d* const pointToStore);
    DAGNode* generateNode(const OrderedSegment* const segmentToStore);
    DAGNode* generateNode(DrawableTrapezoid* const trapezoidToStore);

    DrawableTrapezoid* queryRec(const OrderedSegment& new_segment, DAGNode* const root) const;
};



#endif // DAG_H
