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
    enum nodeType {point, segment, trapezoid};

    // Content of a node
    union info {
        const cg3::Point2d* p;
        const OrderedSegment* s;
        DrawableTrapezoid* t;
    };

    // STATIC NODE GENERATORS
    static DAGNode* generateXNode(const cg3::Point2d* p);
    static DAGNode* generateYNode(const OrderedSegment* s);
    static DAGNode* generateLeafNode(DrawableTrapezoid* t, bool& leafAlreadyPresent);
    // Creates a new generic node
    static DAGNode* newNode(nodeType type, info info);

    // GETTERS
    // get the type stored in the node (point, segment or trapezoid)
    const nodeType& getNodeType() const;
    bool isLeaf() const;
    bool isXNode() const;
    bool isYNode() const;
    const cg3::Point2d* getPointStored() const;
    const OrderedSegment* getOrientedSegmentStored() const;
    DrawableTrapezoid* getTrapezoidStored() const;
    // CONVERTERS
    void convertToXNode(const cg3::Point2d* const p);
    void convertToYNode(const OrderedSegment* const s);
    void convertToLeafNode(DrawableTrapezoid* const t);

    //TODO    DAGNode* father; Maybe I don't need this
    DAGNode* lc;
    DAGNode* rc;

private:
    nodeType type;
    info value;
};

#endif // DAGNODE_H
