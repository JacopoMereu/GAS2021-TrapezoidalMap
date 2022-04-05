#include "dagnode.h"

/*
DAGNode::DAGNode(cg3::Point2* p) {
    DAGNode::info info;
    info.p = p;
    return newNode(point, info);
}*/
// PRIVATE
DAGNode* DAGNode::newNode(nodeType type, info info) {
    DAGNode* new_node = (DAGNode*)malloc(sizeof(DAGNode));
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

DAGNode* DAGNode::generateXNode(cg3::Point2d* p) {
    DAGNode::info info;
    info.p = p;
    return DAGNode::newNode(point, info);
}

DAGNode* DAGNode::generateYNode(OrderedSegment* s) {
    DAGNode::info info;
    info.s = s;
    return DAGNode::newNode(segment, info);
}

DAGNode* DAGNode::generateLeafNode(Trapezoid* t) {
    DAGNode::info info;
    info.t = t;
    return DAGNode::newNode(trapezoid, info);
}

DAGNode::nodeType DAGNode::getNodeType() const {
    return this->type;
}

const cg3::Point2d* DAGNode::getPointStored() {
    if(getNodeType() != DAGNode::point) return nullptr;
    return this->value.p;
}

const OrderedSegment* DAGNode::getOrientedSegmentStored() const {
    if(getNodeType() != DAGNode::segment) return nullptr;
    return this->value.s;
}

Trapezoid* DAGNode::getTrapezoidStored() const {
    if(getNodeType() != DAGNode::trapezoid) return nullptr;
    return this->value.t;
}
