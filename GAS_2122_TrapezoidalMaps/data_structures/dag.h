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
    DAG(Trapezoid* B);

    /* The DAG will contain internal nodes (point/x-node or segment/y-node)
    and leaves (trapezoid).*/
    enum nodeType {point, segment, trapezoid};

    void replaceNodeWithSubtree();
    Trapezoid& query(Point2& q);
private:
    // Content of a node
    union info {
        cg3::Point2* p;
        OrderedSegment* s;
        Trapezoid* t;
    };

    // Generic node structure
    struct Node {
        nodeType type;
        struct Node* lc;
        struct Node* rc;
        info value;
    };

    // root
    Node* root;

    // Creates a new generic node
    Node* newNode(nodeType type, info info);
    Trapezoid& queryRec(cg3::Point2& q, Node* root);
};

#endif // DAG_H
