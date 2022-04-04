#ifndef DAG_H
#define DAG_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
//#include "cg3/geometry/segment2.h"
//#include "cg3/geometry/polygon2.h"
#include "trapezoid.h"
#include "algorithms/OrientationUtility.h"



class DAG
{
public:
    DAG();
    void initialize(Trapezoid& B); // Necessary because the DAG constructor is implicited called at the beginning of the TrapezoidalMap...

    /* The DAG will contain internal nodes (point/x-node or segment/y-node)
    and leaves (trapezoid).*/
    enum nodeType {point, segment, trapezoid};

private:
    // Content of a node
    union info {
        cg3::Point2d* p;
        OrderedSegment* s;
        Trapezoid* t;
    };

public:
    // Generic node structure
    struct Node {
        nodeType type;
        struct Node* lc;
        struct Node* rc;
        info value;
    };
    void replaceNodeWithSubtree(Node* nodeToReplace, OrderedSegment segmentSplitting, std::vector<Trapezoid> newFaces);
    Trapezoid* query(const cg3::Point2d& q);

private:
    // root
    Node* root;

    // Creates a new generic node
    Node* newNode(nodeType type, info info);
    Trapezoid* queryRec(const cg3::Point2d& q, Node* root);
};

#endif // DAG_H
