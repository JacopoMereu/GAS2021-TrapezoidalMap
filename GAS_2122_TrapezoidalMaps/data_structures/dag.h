#ifndef DAG_H
#define DAG_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "algorithms/OrientationUtility.h"
#include "dagnode.h"

//class Trapezoid;

class DAG
{
public:
    DAG();
    void initialize(Trapezoid* B); // Necessary because the DAG constructor is implicited called at the beginning of the TrapezoidalMap...
    void replaceNodeWithSubtree(DAGNode* nodeToReplace, OrderedSegment& segmentSplitting,
                                Trapezoid* left=nullptr, Trapezoid* top=nullptr, Trapezoid* bottom=nullptr, Trapezoid* right=nullptr);
    Trapezoid* query(const cg3::Point2d& q);
    void clear();

private:
    // root
    DAGNode* root;
    Trapezoid* queryRec(const cg3::Point2d& q, DAGNode* root);
    void clearRec(DAGNode* root);
};



#endif // DAG_H
