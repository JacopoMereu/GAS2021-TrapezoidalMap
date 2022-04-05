#include "dag.h"

/*
 *  Set the DAG with a leaf containing the bounding box
*/
DAG::DAG()
{
    assert (this->root == nullptr);
}

void DAG::initialize(Trapezoid& B) {
    assert (root == nullptr);
    info infoB; infoB.t = &B;
    root = newNode(trapezoid, infoB);
}


void DAG::replaceNodeWithSubtree(Node* nodeToReplace, OrderedSegment segmentSplitting, std::vector<Trapezoid> newFaces) {

}


Trapezoid* DAG::query(const cg3::Point2d& q) {
    return queryRec(q, this->root);
}

// PRIVATE SECTION
DAG::Node* DAG::newNode(nodeType type, info info) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    assert(new_node != nullptr);

    new_node->type = type;
    new_node->lc = nullptr;
    new_node->rc = nullptr;
    new_node->value = info;

    if(type == trapezoid) {
        /*assert(info.t->getPointerToDAG() == nullptr); // should be null
            info.t->setPointerToDAG(new_node);*/
        ;
    }
    return new_node;
}


Trapezoid* DAG::queryRec(const cg3::Point2d& q, Node* node) {
    switch (node->type) {
    // x-node
    case point: {
        // q.x < node.x => go left
        if(q.x() < node->value.p->x()) {
            return queryRec(q, node->lc);
        }
        // q.x > node.x => go right
        else if (q.x() > node->value.p->x()) {
            return queryRec(q, node->rc);
        }
        //TODO Should I handle q.x == node.x =?
        break;
    }
        // y-node
    case segment: {
        Position pos = OrientationUtility::getPointPositionRespectToLine(q, *(node->value.s));
        switch (pos) {

        // q above segment => go left
        case above:
            return queryRec(q, node->lc);
            break;

            // q below segment => go right
        case below:
            return queryRec(q, node->rc);
            break;

            //TODO Should I handle q on the segment =?
            /*case center:
            break;*/
        default:
            assert(false);
            break;
        }

        break;
    }

        // leaf
    case trapezoid: {
        // if we reach a leaf, the point is contained in the trapezoid associated to the node
        return node->value.t;
        break;
    }
        // shpuld be impossible
    default:
        assert(false);
        break;
    }

    assert(false);
    std::exit(EXIT_FAILURE);
}
// END PRIVATE SECTION
