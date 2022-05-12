#include "dagnode.h"



/////////// STATIC NODE GENERATORS ////////////////////////////
DAGNode* DAGNode::generateXNode(const cg3::Point2d* const p) {
    DAGNode::info info;
    info.p = p;
    return DAGNode::newNode(x_node, info);
}

DAGNode* DAGNode::generateYNode(const OrderedSegment* const s) {
    DAGNode::info info;
    info.s = s;
    return DAGNode::newNode(y_node, info);
}

DAGNode* DAGNode::generateLeafNode(DrawableTrapezoid* const t,  bool& leafAlreadyPresent) {
    // If it exists already a node containing the trapezoid, return that node
    if (t->getPointerToDAG() != nullptr) {
        leafAlreadyPresent = true;
        return t->getPointerToDAG();
    }

    // otherwise create a new node
    leafAlreadyPresent = false;
    DAGNode::info info;
    info.t = t;
    return DAGNode::newNode(leaf, info);
}

DAGNode* DAGNode::newNode(nodeType type, info info) {
    DAGNode* new_node = new DAGNode;
    assert(new_node != nullptr);

    new_node->type = type;
    new_node->lc = nullptr;
    new_node->rc = nullptr;
    new_node->value = info;

    // If I'm creating a leaf
    if(type == leaf) {
        assert(info.t->getPointerToDAG() == nullptr); // there shouldn't exist a leaf in the dag pointing to the trapezoid, otherwise I wouldn't create this node
        info.t->setPointerToDAG(new_node);

    }
    return new_node;
}
////////////////////////////////////////////////////////////////////////////////////



//////////////////////////// GETTERS ////////////////////////////////////////////////////////
const DAGNode::nodeType& DAGNode::getNodeType() const {
    return this->type;
}
bool DAGNode::isLeaf() const {
    return getNodeType() == leaf;
}
bool DAGNode::isXNode() const {
    return getNodeType() == x_node;
}
bool DAGNode::isYNode() const {
    return getNodeType() == y_node;
}
const cg3::Point2d* DAGNode::getPointStored() const {
    if(!isXNode()) return nullptr;
    return this->value.p;
}
const OrderedSegment* DAGNode::getOrientedSegmentStored() const {
    if(!isYNode()) return nullptr;
    return this->value.s;
}
DrawableTrapezoid* DAGNode::getTrapezoidStored() const {
    if(!isLeaf()) return nullptr;
    return this->value.t;
}
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////// CONVERTERS ////////////////////////////
void DAGNode::convertToXNode(const cg3::Point2d* const p) {
    this->type = x_node;
    this->value.p = p;

}
void DAGNode::convertToYNode(const OrderedSegment* const s) {
    this->type = y_node;
    this->value.s = s;
}
void DAGNode::convertToLeafNode(DrawableTrapezoid* const t) {
    this->type = leaf;
    this->value.t = t;
}
//////////////////////////////////////////////////////////////////////////////////////////
