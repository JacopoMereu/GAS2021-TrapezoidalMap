#ifndef DAG_H
#define DAG_H

#include "cg3/geometry/point2.h"
#include "cg3/geometry/segment2.h"
//#include "cg3/geometry/polygon2.h"
#include "trapezoid.h"

class DAG
{
public:
    DAG(Trapezoid* B);

    /* The DAG will contain internal nodes (point/x-node or segment/y-node)
    and leaves (trapezoid).*/
    enum nodeType {point, segment, trapezoid};

    void replaceNodeWithSubtree();

private:
    // Content of a node
    union info {
        cg3::Point2* p;
        cg3::Segment2* s;
        Trapezoid* t; //TODO Do I need a trapezoid class???
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
};

#endif // DAG_H
