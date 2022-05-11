#include "dag.h"

#include "cg3/geometry/utils2.h"
#include "cg3/geometry/line2.h"
/*
 *  Set the DAG with a leaf containing the bounding box
*/
DAG::DAG()
{
    assert (this->root == nullptr);
}

void DAG::initialize(DrawableTrapezoid* B) {
    assert (root == nullptr);
    assert(B->getPointerToDAG() == nullptr);

    root = generateNode(B);
}


void DAG::replaceNodeWithSubtree(DAGNode* nodeToReplace, OrderedSegment& segmentSplitting, DrawableTrapezoid* left, DrawableTrapezoid* top, DrawableTrapezoid* bottom, DrawableTrapezoid* right) {
    // Double check if the node is a leaf
    assert(nodeToReplace->lc == nullptr);
    assert(nodeToReplace->rc == nullptr);
    assert(nodeToReplace->isLeaf());

    // Creating the segment subtree (it's always created in every case)
    auto segmentNode = generateNode(&segmentSplitting);
    segmentNode->lc = generateNode(top);
    segmentNode->rc = generateNode(bottom);

    /* SIMPLE CASE: the whole segment is inside a face */
    if(left != nullptr && right != nullptr) {
        nodeToReplace->lc = generateNode(left);
        nodeToReplace->rc = generateNode(&segmentSplitting.getRightmost());
        nodeToReplace->rc->lc = segmentNode;
        nodeToReplace->rc->rc = generateNode(right);
        nodeToReplace->convertToXNode(&segmentSplitting.getLeftmost());
    }
    /* COMPLEX CASE: several faces intersected by the segment */
    // If it's the first face
    else if (left != nullptr) {
        nodeToReplace->lc = generateNode(left);
        nodeToReplace->rc = segmentNode;
        nodeToReplace->convertToXNode(&segmentSplitting.getLeftmost());
    }
    // If it's the last (k-th) face
    else if (right != nullptr) {
        nodeToReplace->lc = segmentNode;
        nodeToReplace->rc = generateNode(right);
        nodeToReplace->convertToXNode(&segmentSplitting.getRightmost());
    }
    // Else it's a i-th face with i in [2, k-1]
    else {
        nodeToReplace->lc = segmentNode->lc;
        nodeToReplace->rc = segmentNode->rc;
        nodeToReplace->convertToYNode(&segmentSplitting);
    }

    if(left!=nullptr)
        assert(left->getPointerToDAG()!=nullptr);
    if(right!=nullptr)
        assert(right->getPointerToDAG()!=nullptr);
    assert(top->getPointerToDAG()!=nullptr);
    assert(bottom->getPointerToDAG()!=nullptr);
}


DrawableTrapezoid* DAG::queryFaceContaininingPoint(const cg3::Point2d& q) {
    OrderedSegment s = OrderedSegment(q,q);
    return queryRec(s, this->root);
}

DrawableTrapezoid* DAG::queryLeftmostFaceIntersectingSegment(const OrderedSegment& s) {
    return queryRec(s, this->root);
}


DAG::~DAG() {
    for (auto iterable_dagnode=uniquePointers.begin(); iterable_dagnode != uniquePointers.end(); iterable_dagnode++)
        if(*iterable_dagnode)
            delete *iterable_dagnode;
    uniquePointers.clear();
    uniquePointers.shrink_to_fit();
    this->root=nullptr;
}
// PRIVATE SECTION
DAGNode* DAG::generateNode(const cg3::Point2d* pointToStore) {
    auto pointNode = DAGNode::generateXNode(pointToStore);
    uniquePointers.push_back(pointNode);
    return pointNode;
}

DAGNode* DAG::generateNode(const OrderedSegment* segmentToStore) {
    auto segmentNode = DAGNode::generateYNode(segmentToStore);
    uniquePointers.push_back(segmentNode);
    return segmentNode;
}
DAGNode* DAG::generateNode(DrawableTrapezoid* trapezoidToStore) {
    bool alreadyPresent;
    auto trapezoidNode = DAGNode::generateLeafNode(trapezoidToStore, alreadyPresent);
        if(!alreadyPresent)
            uniquePointers.push_back(trapezoidNode);
    return trapezoidNode;
}

DrawableTrapezoid* DAG::queryRec(const OrderedSegment& new_segment, DAGNode* node) {
    auto q = new_segment.getLeftmost();

    switch (node->getNodeType()) {
    // x-node
    case DAGNode::point: {
        // q.x < node.x => go left
        auto p_x = node->getPointStored()->x();
        if(q.x() < p_x) {
            return queryRec(new_segment, node->lc);
        }
        // q.x > node.x => go right
        else if (q.x() > p_x) {
            return queryRec(new_segment, node->rc);
        }
        //TODO Should I handle q.x == node.x =?
        return queryRec(new_segment, node->rc);
        assert(false);
        break;
    }
        // y-node
    case DAGNode::segment: {
        // q above segment => go left
        if(cg3::isPointAtLeft(node->getOrientedSegmentStored()->getLeftmost(), node->getOrientedSegmentStored()->getRightmost(), q)){
            return queryRec(new_segment, node->lc);
        }
        // q below segment => go right
        else if(cg3::isPointAtRight(node->getOrientedSegmentStored()->getLeftmost(), node->getOrientedSegmentStored()->getRightmost(), q)) {
            return queryRec(new_segment, node->rc);
        }
        // q ON THE SEGMENT
        else {
            //assert(false);
            //TODO Should I handle q on the segment =?
            double m_old_segment = cg3::Line2(*node->getOrientedSegmentStored()).m();
            double m_new_segment = cg3::Line2(new_segment).m();
            // m(new_segment) > m(old_segment) => q lies above
            if(m_new_segment > m_old_segment)
                return queryRec(new_segment, node->lc);
            // m(new_segment) > m(old_segment) => q lies below
            else if (m_new_segment < m_old_segment)
                return queryRec(new_segment, node->rc);

            // same m??
            assert(false);
        }
        break;
    }

        // leaf
    case DAGNode::trapezoid: {
        // if we reach a leaf, the point is contained in the trapezoid associated to the node
        return node->getTrapezoidStored();
    }
        // shpuld be impossible
    default:
        assert(false);
    }

    assert(false);
    std::exit(EXIT_FAILURE);
}
// END PRIVATE SECTION
