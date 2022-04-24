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


void DAG::replaceNodeWithSubtree(DAGNode* nodeToReplace, OrderedSegment& segmentSplitting, std::vector<Trapezoid*> newFaces) {
    // Double check if the node is a leaf
    assert(nodeToReplace->lc == nullptr);
    assert(nodeToReplace->rc == nullptr);
    assert(nodeToReplace->isLeaf());

    //TODO a better way to pass the new faces
    // create the node containing the leftmost endpoint of the segment. this node is the new root of the subtree
    auto newRoot = DAGNode.generateXNode(segmentSplitting.getLeftmost());
    // create the node containing the left face and attach it to the root
    newRoot->lc = DAGNode.generateLeafNode(newFaces.at(0));
    // create the node containing the right endpoint and attach it to the root
    auto tmp = DAGNode.generateXNode(segmentSplitting.getRightmost());
    newRoot->rc = tmp;
    // create the node containing the right face and attach it to the rightendpoint node
    tmp->rc = DAGNode.generateLeafNode(newFaces.at(2));
    // create the node containing the segment and attach it to the rightendpoint node
    tmp->lc = DAGNode.generateYNode(segmentSplitting);
    tmp = tmp->lc;
    // create the node containing the top face and attach it to the segment node
    tmp->lc = DAGNode.generateLeafNode(newFaces.at(1));
    tmp->rc = DAGNode.generateLeafNode(newFaces.at(3));

    // replace
}


Trapezoid* DAG::query(const cg3::Point2d& q) {
    return queryRec(q, this->root);
}

// PRIVATE SECTION


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
        break;
    }
        // y-node
    case DAGNode::segment: {
        Position pos = OrientationUtility::getPointPositionRespectToLine(q, *(node->getOrientedSegmentStored()));
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
    case DAGNode::trapezoid: {
        // if we reach a leaf, the point is contained in the trapezoid associated to the node
        return node->getTrapezoidStored();
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
