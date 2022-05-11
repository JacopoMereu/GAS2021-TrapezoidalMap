#ifndef DAG_H
#define DAG_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "dagnode.h"

class DAG
{
public:
    DAG();
    ~DAG();

    void initialize(DrawableTrapezoid* B);
    void replaceNodeWithSubtree(DAGNode* nodeToReplace, OrderedSegment& segmentSplitting,
                                DrawableTrapezoid* left=nullptr, DrawableTrapezoid* top=nullptr, DrawableTrapezoid* bottom=nullptr, DrawableTrapezoid* right=nullptr);
    DrawableTrapezoid* queryFaceContaininingPoint(const cg3::Point2d& q);
    DrawableTrapezoid* queryLeftmostFaceIntersectingSegment(const OrderedSegment& s);


private:
    // root
    DAGNode* root = nullptr;
    std::vector<DAGNode*> uniquePointers;
    DAGNode* generateNode(const cg3::Point2d* pointToStore);
    DAGNode* generateNode(const OrderedSegment* segmentToStore);
    DAGNode* generateNode(DrawableTrapezoid* trapezoidToStore);

    DrawableTrapezoid* queryRec(const OrderedSegment& new_segment, DAGNode* root);
};



#endif // DAG_H
