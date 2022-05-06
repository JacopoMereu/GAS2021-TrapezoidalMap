#ifndef DAGNODE_H
#define DAGNODE_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "drawables/drawabletrapezoid.h"
#include "algorithms/OrientationUtility.h"

//class Trapezoid;

class DAGNode
{
public:
    //DAGNode(cg3::Point2* p);
    //~DAGNode();
    enum nodeType {point, segment, trapezoid};

    // Content of a node
    union info {
        const cg3::Point2d* p;
        const OrderedSegment* s;
        DrawableTrapezoid* t;
    };

    /*void setNodeAsXNode(cg3::Point2d& t);
    void setNodeAsYNode(OrderedSegment& t);
    void setNodeAsLeafNode(Trapezoid& t);*/

    static DAGNode* generateXNode(const cg3::Point2d* p);
    static DAGNode* generateYNode(OrderedSegment* s);
    static DAGNode* generateLeafNode(DrawableTrapezoid* t);
    // Creates a new generic node
    static DAGNode* newNode(nodeType type, info info);

    nodeType getNodeType() const;
    bool isLeaf() const;
    bool isXNode() const;
    bool isYNode() const;
    const cg3::Point2d* getPointStored();
    const OrderedSegment* getOrientedSegmentStored() const;
    DrawableTrapezoid* getTrapezoidStored() const;
    void convertToXNode(const cg3::Point2d* p);
    void convertToYNode(const OrderedSegment* s);
    void convertToLeafNode(DrawableTrapezoid* t);

    //TODO    DAGNode* father; Maybe I don't need this
    DAGNode* lc;
    DAGNode* rc;

private:
    nodeType type;
    info value;
};

#endif // DAGNODE_H
