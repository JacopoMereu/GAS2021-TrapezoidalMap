#include "dagnode.h"


// PRIVATE
DAGNode* DAGNode::newNode(nodeType type, info info) {
    DAGNode* new_node = new DAGNode;
    assert(new_node != nullptr);

    new_node->type = type;
    new_node->lc = nullptr;
    new_node->rc = nullptr;
    new_node->value = info;

    if(type == trapezoid) {
        assert(info.t->getPointerToDAG() == nullptr); // should be null
        info.t->setPointerToDAG(new_node);

    }
    return new_node;
}

// PUBLIC

DAGNode* DAGNode::generateXNode(const cg3::Point2d* p) {
    DAGNode::info info;
    info.p = p;
    return DAGNode::newNode(point, info);
}

DAGNode* DAGNode::generateYNode(const OrderedSegment* s) {
    DAGNode::info info;
    info.s = s;
    return DAGNode::newNode(segment, info);
}

DAGNode* DAGNode::generateLeafNode(DrawableTrapezoid* t,  bool& leafAlreadyPresent) {
    // If it exists already a node containing the trapezoid, return that node
    if (t->getPointerToDAG() != nullptr) {
        leafAlreadyPresent = true;
        return t->getPointerToDAG();
    }

    // otherwise create a new node
    leafAlreadyPresent = false;
    DAGNode::info info;
    info.t = t;
    return DAGNode::newNode(trapezoid, info);
}

const DAGNode::nodeType& DAGNode::getNodeType() const {
    return this->type;
}

bool DAGNode::isLeaf() const {
    return getNodeType() == trapezoid;
}
bool DAGNode::isXNode() const {
    return getNodeType() == point;
}
bool DAGNode::isYNode() const {
    return getNodeType() == segment;
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

void DAGNode::convertToXNode(const cg3::Point2d* const p) {
    this->type = point;
    this->value.p = p;

}
void DAGNode::convertToYNode(const OrderedSegment* const s) {
    this->type = segment;
    this->value.s = s;
}
void DAGNode::convertToLeafNode(DrawableTrapezoid* const t) {
    this->type = trapezoid;
    this->value.t = t;
}
