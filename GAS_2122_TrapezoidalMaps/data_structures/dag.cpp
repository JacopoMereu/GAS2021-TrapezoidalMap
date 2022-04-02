#include "dag.h"

/*
 *  Set the DAG with a leaf containing the bounding box
*/
DAG::DAG(Trapezoid* B)
{
    assert (root == nullptr);
    root = newNode(nodeType.trapezoid, B);
}

DAG::newNode(nodeType type, info info) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        assert(new_node != nullptr);

        new_node->type = type;
        new_node->lc = nullptr;
        new_node->rc = nullptr;
        new_node->value = info;

        return new_node;
}


DAG::replaceNodeWithSubtree() {

}
