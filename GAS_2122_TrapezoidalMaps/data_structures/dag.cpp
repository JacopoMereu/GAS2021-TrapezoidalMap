#include "dag.h"

/*
 *  Set the DAG with a leaf containing the bounding box
*/
DAG::DAG(cg3::BoundingBox2& B)
{
    assert (root == nullptr);
    // TODO Convert bb into polygon?
    root = newNode(nodeType.trapezoid, (Poligon2d*) B);
}

DAG::newNode(nodeType type, info info) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        assert(new_node != nullptr);

        new_node->type = type;
        new_node->lc = nullptr;
        new_node->rc = nullptr;


        return new_node;
}


DAG::replaceNodeWithSubtree() {

}
