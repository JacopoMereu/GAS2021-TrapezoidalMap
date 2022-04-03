#include "dag.h"

/*
 *  Set the DAG with a leaf containing the bounding box
*/
DAG::DAG(Trapezoid* B)
{
    assert (root == nullptr);
    root = newNode(nodeType.trapezoid, B);
}




DAG::replaceNodeWithSubtree() {

}


Trapezoid& DAG::query(Point2& q) {
    return queryRec(q, this->root);
}

// PRIVATE SECTION
DAG::newNode(nodeType type, info info) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        assert(new_node != nullptr);

        new_node->type = type;
        new_node->lc = nullptr;
        new_node->rc = nullptr;
        new_node->value = info;

        return new_node;
}


Trapezoid& DAG::queryRec(cg3::Point2& q, Node* root) {
    switch (root->type) {
    // x-node
    case point:
        // q.x < root.x => go left
        if(q.x() < root->value.p->x()) {
            return queryRec(q, root->lc);
        }
        // q.x > root.x => go right
        else if (q.x() > root->value.p->x()) {
            return queryRec(q, root->rc);
        }
        //TODO Should I handle q.x == root.x =?
        break;

    // y-node
    case segment:
        Position pos = getPointPositionRespectToLine(q, root->value.s);
        switch (pos) {

        // q above segment => go left
        case left:
            return queryRec(q, root->lc);
            break;

        // q below segment => go right
        case right:
            return queryRec(q, root->rc);
            break;

        //TODO Should I handle q on the segment =?
        /*case center:
            break;*/
        default:
            assert(false);
            break;
        }

        break;

    // leaf
    case trapezoid:
        // if we reach a leaf, the point is contained in the trapezoid associated to the node
        return this->info.t;
        break;

    // shpuld be impossible
    default:
        assert(false);
        break;
    }
}
// END PRIVATE SECTION
