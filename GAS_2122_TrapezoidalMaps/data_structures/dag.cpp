#include "dag.h"

/*
 *  Set the DAG with a leaf containing the bounding box
*/
DAG::DAG()
{
    assert (this->root == nullptr);
}

void DAG::initialize(Trapezoid* B) {
    assert (root == nullptr);
//    info infoB; infoB.t = &B;
//    root = newNode(trapezoid, infoB);
    root = DAGNode::generateLeafNode(B);
}


void DAG::replaceNodeWithSubtree(DAGNode* nodeToReplace, OrderedSegment& segmentSplitting, Trapezoid* left, Trapezoid* top, Trapezoid* bottom, Trapezoid* right) {
    // Double check if the node is a leaf
    assert(nodeToReplace->lc == nullptr);
    assert(nodeToReplace->rc == nullptr);
    assert(nodeToReplace->isLeaf());

    //TODO a better way to pass the new faces
    // Creating the segment subtree (it's always created in every case)
    auto segmentNode = DAGNode::generateYNode(&segmentSplitting);
    segmentNode->lc = DAGNode::generateLeafNode(top);
    segmentNode->rc = DAGNode::generateLeafNode(bottom);
    DAGNode* newRoot;
    /* COMPLEX CASE: the whole segment is inside a face */
    if(left != nullptr && right != nullptr) {
        newRoot = DAGNode::generateXNode(&segmentSplitting.getLeftmost());
        nodeToReplace->lc = DAGNode::generateLeafNode(left);
        nodeToReplace->rc = DAGNode::generateXNode(&segmentSplitting.getRightmost());
        nodeToReplace->rc->lc = segmentNode;
        nodeToReplace->rc->rc = DAGNode::generateLeafNode(right);
        // replace the old node with the new node
        nodeToReplace->convertToXNode(newRoot->getPointStored());
    }
    /* COMPLEX CASE: several faces intersected by the segment */
    // If it's the first face
    else if (left != nullptr) {
        newRoot = DAGNode::generateXNode(&segmentSplitting.getLeftmost());
        nodeToReplace->lc = DAGNode::generateLeafNode(left);
        nodeToReplace->rc = segmentNode;
        nodeToReplace->convertToXNode(newRoot->getPointStored());
    }
    // If it's the last (k-th) face
    else if (right != nullptr) {
        newRoot = DAGNode::generateXNode(&segmentSplitting.getRightmost());
        nodeToReplace->lc = segmentNode;
        nodeToReplace->rc = DAGNode::generateLeafNode(right);
        nodeToReplace->convertToXNode(newRoot->getPointStored());
    }
    // Else it's a i-th face with i in [2, k-1]
    else {
        newRoot = segmentNode;
        nodeToReplace->lc = segmentNode->lc;
        nodeToReplace->rc = segmentNode->rc;
        nodeToReplace->convertToYNode(newRoot->getOrientedSegmentStored());
    }
    delete newRoot;
}


Trapezoid* DAG::query(const cg3::Point2d& q) {
    return queryRec(q, this->root);
}

void DAG::clear() {
    clear();
}
// PRIVATE SECTION
void DAG::clearRec(DAGNode* root) {
    if(root == nullptr) return;

    clearRec(root->lc);
    clearRec(root->rc);

    delete root;
}

Trapezoid* DAG::queryRec(const cg3::Point2d& q, DAGNode* node) {
    switch (node->getNodeType()) {
    // x-node
    case DAGNode::point: {
        // q.x < node.x => go left
        auto p_x = node->getPointStored()->x();
        if(q.x() < p_x) {
            return queryRec(q, node->lc);
        }
        // q.x > node.x => go right
        else if (q.x() > p_x) {
            return queryRec(q, node->rc);
        }
        //TODO Should I handle q.x == node.x =?
        assert(false);
        break;
    }
        // y-node
    case DAGNode::segment: {
        Position pos = OrientationUtility::getPointPositionRespectToLine(q, *(node->getOrientedSegmentStored()));
        switch (pos) {

        // q above segment => go left
        case above:
            return queryRec(q, node->lc);

            // q below segment => go right
        case below:
            return queryRec(q, node->rc);

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
