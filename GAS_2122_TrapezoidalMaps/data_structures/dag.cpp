#include "dag.h"

#include "cg3/geometry/utils2.h"
#include "cg3/geometry/line2.h"

/// CONSTRUCTOR AND DESTRUCTOR ///
DAG::DAG()
{
    assert (this->root == nullptr);
}

DAG::~DAG() {
    // delete the nodes
    for (auto iterable_dagnode=uniquePointers.begin(); iterable_dagnode != uniquePointers.end(); iterable_dagnode++)
        if(*iterable_dagnode)
            delete *iterable_dagnode;
    // clear and resize the list
    uniquePointers.clear();
    uniquePointers.shrink_to_fit();

    // set the root to null
    this->root=nullptr;
}
///////////////////////////////////////


void DAG::initialize(DrawableTrapezoid* const B) {
    assert (root == nullptr);
    assert(B->getPointerToDAG() == nullptr);

    root = generateNode(B);
}

void DAG::replaceNodeWithSubtree(DAGNode* const leafToUpdate, const OrderedSegment& segmentSplitting, DrawableTrapezoid* const leftFace, DrawableTrapezoid* const topFace, DrawableTrapezoid* const bottomFace, DrawableTrapezoid* const rightFace) {
    // Double check if the node is a leaf
    assert(leafToUpdate->lc == nullptr);
    assert(leafToUpdate->rc == nullptr);
    assert(leafToUpdate->isLeaf());

    // Top and bottom faces must not be null
    assert(topFace != nullptr);
    assert(bottomFace != nullptr);

    // Creating the segment subtree (it's always created in every case)
    auto segmentNode = generateNode(&segmentSplitting);
    segmentNode->lc = generateNode(topFace);
    segmentNode->rc = generateNode(bottomFace);

    /* SIMPLE CASE: THE WHOLE SEGMENT IS INSIDE A FACE */
    if(leftFace != nullptr && rightFace != nullptr) {
        leafToUpdate->lc = generateNode(leftFace);
        leafToUpdate->rc = generateNode(&segmentSplitting.getRightmost());
        leafToUpdate->rc->lc = segmentNode;
        leafToUpdate->rc->rc = generateNode(rightFace);
        leafToUpdate->convertToXNode(&segmentSplitting.getLeftmost());
    }

    /* COMPLEX CASE: SEVERAL FACES ARE INTERSECTED BY THE SEGMENT AND leafToUpdate IS ONE OF THEM */
    // If leafToUpdate is the first face intersected AND the segment is not intersecting the leftp of the old face.
    else if (leftFace != nullptr) {
        leafToUpdate->lc = generateNode(leftFace);
        leafToUpdate->rc = segmentNode;
        leafToUpdate->convertToXNode(&segmentSplitting.getLeftmost());
    }
    // If it's the last face (k-th) intersected AND the segment is not intersecting the rightp of the old face.
    else if (rightFace != nullptr) {
        leafToUpdate->lc = segmentNode;
        leafToUpdate->rc = generateNode(rightFace);
        leafToUpdate->convertToXNode(&segmentSplitting.getRightmost());
    }
    /* Else it's a i-th face with i in [2, k-1]
     *      OR the first face AND the segment is intersecting the leftp of the old face
     *      OR the last  face AND the segment is intersecting the rightp of the old face */
    else {
        leafToUpdate->lc = segmentNode->lc;
        leafToUpdate->rc = segmentNode->rc;
        leafToUpdate->convertToYNode(&segmentSplitting);
    }

    // Asserting the double links
    if(leftFace!=nullptr)
        assert(leftFace->getPointerToDAG()!=nullptr);
    if(rightFace!=nullptr)
        assert(rightFace->getPointerToDAG()!=nullptr);
    assert(topFace->getPointerToDAG()!=nullptr);
    assert(bottomFace->getPointerToDAG()!=nullptr);
}


DrawableTrapezoid* DAG::queryFaceContaininingPoint(const cg3::Point2d& q) const {
    OrderedSegment s = OrderedSegment(q,q);
    return queryRec(s, this->root);
}

DrawableTrapezoid* DAG::queryLeftmostFaceIntersectingSegment(const OrderedSegment& s) const {
    return queryRec(s, this->root);
}

//////////////////////////////////////////////// NODE GENERATORS ////////////////////////////////
DAGNode* DAG::generateNode(const cg3::Point2d* const pointToStore) {
    auto pointNode = DAGNode::generateXNode(pointToStore);
    uniquePointers.push_back(pointNode);
    return pointNode;
}

DAGNode* DAG::generateNode(const OrderedSegment* const segmentToStore) {
    auto segmentNode = DAGNode::generateYNode(segmentToStore);
    uniquePointers.push_back(segmentNode);
    return segmentNode;
}
DAGNode* DAG::generateNode(DrawableTrapezoid* const trapezoidToStore) {
    bool alreadyPresent;
    auto trapezoidNode = DAGNode::generateLeafNode(trapezoidToStore, alreadyPresent);
    // If a leaf containing the trapezoid was already present, do NOT push the node (again) in the list
    if(!alreadyPresent)
            uniquePointers.push_back(trapezoidNode);
    return trapezoidNode;
}
////////////////////////////////////////////////////////////////////////////////////////////////

DrawableTrapezoid* DAG::queryRec(const OrderedSegment& new_segment, DAGNode* const node) const {
    auto q = new_segment.getLeftmost();

    if(node->isXNode()) {
        // q.x < node.x => go left
        auto p_x = node->getPointStored()->x();
        if(q.x() < p_x) {
            return queryRec(new_segment, node->lc);
        }
        // q.x >= node.x => go right
        else {
            return queryRec(new_segment, node->rc);
        }
    }
    else if (node->isYNode()) {
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
            // slopes of the (line passing through the) old segment
            double m_old_segment = cg3::Line2(*node->getOrientedSegmentStored()).m();
            // slopes of the (line passing through the) new segment
            double m_new_segment = cg3::Line2(new_segment).m();

            // slope(new_segment) > slope(old_segment) => q lies above
            if(m_new_segment > m_old_segment)
                return queryRec(new_segment, node->lc);
            // slope(new_segment) < slope(old_segment) => q lies below
            else if (m_new_segment < m_old_segment)
                return queryRec(new_segment, node->rc);

            // same slope should be impossible
            assert(false);
        }
    }

    // else we reached a leaf, the point is contained in the trapezoid associated to the node
    return node->getTrapezoidStored();
}
