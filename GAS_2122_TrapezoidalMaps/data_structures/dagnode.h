#ifndef DAGNODE_H
#define DAGNODE_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "trapezoid.h"
#include "algorithms/OrientationUtility.h"

//class Trapezoid;

class DAGNode
{
public:
    //DAGNode(cg3::Point2* p);
    enum nodeType {point, segment, trapezoid};

    // Content of a node
    union info {
        cg3::Point2d* p;
        OrderedSegment* s;
        Trapezoid* t;
    };

    /*void setNodeAsXNode(cg3::Point2d& t);
    void setNodeAsYNode(OrderedSegment& t);
    void setNodeAsLeafNode(Trapezoid& t);*/

    static DAGNode* generateXNode(cg3::Point2d* p);
    static DAGNode* generateYNode(OrderedSegment* s);
    static DAGNode* generateLeafNode(Trapezoid* t);
    // Creates a new generic node
    static DAGNode* newNode(nodeType type, info info);

    nodeType getNodeType() const;
    bool isLeaf() const;
    bool isXNode() const;
    bool isYNode() const;
    const cg3::Point2d* getPointStored();
    const OrderedSegment* getOrientedSegmentStored() const;
    Trapezoid* getTrapezoidStored() const;

    DAGNode* lc;
    DAGNode* rc;

private:
    nodeType type;
    info value;
};

#endif // DAGNODE_H
