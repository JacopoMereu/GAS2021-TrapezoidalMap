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
    root = DAGNode::generateLeafNode(B);
}


void DAG::replaceNodeWithSubtree(DAGNode* nodeToReplace, OrderedSegment& segmentSplitting, DrawableTrapezoid* left, DrawableTrapezoid* top, DrawableTrapezoid* bottom, DrawableTrapezoid* right) {
    // Double check if the node is a leaf
    assert(nodeToReplace->lc == nullptr);
    assert(nodeToReplace->rc == nullptr);
    assert(nodeToReplace->isLeaf());

    //TODO a better way to pass the new faces
    // Creating the segment subtree (it's always created in every case)
    auto segmentNode = DAGNode::generateYNode(&segmentSplitting);
    segmentNode->lc = DAGNode::generateLeafNode(top);
    segmentNode->rc = DAGNode::generateLeafNode(bottom);

    /* COMPLEX CASE: the whole segment is inside a face */
    if(left != nullptr && right != nullptr) {
        //newRoot = DAGNode::generateXNode();
        nodeToReplace->lc = DAGNode::generateLeafNode(left);
        nodeToReplace->rc = DAGNode::generateXNode(&segmentSplitting.getRightmost());
        nodeToReplace->rc->lc = segmentNode;
        nodeToReplace->rc->rc = DAGNode::generateLeafNode(right);
        // replace the old node with the new node
        nodeToReplace->convertToXNode(&segmentSplitting.getLeftmost());
    }
    /* COMPLEX CASE: several faces intersected by the segment */
    // If it's the first face
    else if (left != nullptr) {
        //newRoot = DAGNode::generateXNode(&segmentSplitting.getLeftmost());
        nodeToReplace->lc = DAGNode::generateLeafNode(left);
        nodeToReplace->rc = segmentNode;
        nodeToReplace->convertToXNode(&segmentSplitting.getLeftmost());
    }
    // If it's the last (k-th) face
    else if (right != nullptr) {
        //newRoot = DAGNode::generateXNode(&segmentSplitting.getRightmost());
        nodeToReplace->lc = segmentNode;
        nodeToReplace->rc = DAGNode::generateLeafNode(right);
        nodeToReplace->convertToXNode(&segmentSplitting.getRightmost());
    }
    // Else it's a i-th face with i in [2, k-1]
    else {
        //newRoot = segmentNode;
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
    //delete newRoot;
}


DrawableTrapezoid* DAG::queryFaceContaininingPoint(const cg3::Point2d& q) {
    OrderedSegment s = OrderedSegment(q,q);
    return queryRec(s, this->root);
}

DrawableTrapezoid* DAG::queryLeftmostFaceIntersectingSegment(const OrderedSegment& s) {
    return queryRec(s, this->root);
}


void DAG::clear() {
    clearRec(this->root);
    this->root=nullptr;
}
// PRIVATE SECTION
void DAG::clearRec(DAGNode* root) {
    if(root == nullptr) return;

    delete root->lc;
    delete root->rc;

    if(root)
        delete root;
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
