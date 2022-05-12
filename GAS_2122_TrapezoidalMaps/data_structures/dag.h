#ifndef DAG_H
#define DAG_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "dagnode.h"

class DAG
{
public:
    // Constructor
    DAG();
    // Destructor: it'll deallocate the nodes dynamically allocated
    ~DAG();

    // initialize the DAG using a trapezoid rapresenting the bounding box
    void initialize(DrawableTrapezoid* const B);

    /**
     * @brief replaceNodeWithSubtree updates the DAG replacing a leaf with a new subtree made up by nodes containing:
     *          the segment, its endpoints (optional, it depends from the case) and from 2 to 4 trapezoids.
     * @param leafToUpdate          the leaf to update
     * @param segmentSplitting      the segment splitting the trapezoid pointed by the leaf
     * @param leftFace              the new left face obtained after the splitting (it can be null)
     * @param topFace               the new top face obtained after the splitting (it CANNOT be null)
     * @param bottomFace            the new bottom face obtained after the splitting (it CANNOT be null)
     * @param rightFace             the new right face obtained after the splitting (it can be null)
     */
    void replaceNodeWithSubtree(DAGNode* const leafToUpdate, const OrderedSegment& segmentSplitting,
                                DrawableTrapezoid* const leftFace, DrawableTrapezoid* const topFace,
                                DrawableTrapezoid* const bottomFace, DrawableTrapezoid* const rightFace);

    /**
     * @brief queryFaceContaininingPoint visits the DAG searching for the trapezoid containing the point q.
     * @param q         the query point.
     * @return          the trapezoid containing the point q.
     */
    DrawableTrapezoid* queryFaceContaininingPoint(const cg3::Point2d& q) const;

    /**
     * @brief queryLeftmostFaceIntersectingSegment visits the DAG searching for the trapezoid containing the leftmost endpoint of a given (ordered)segment
     * @param s         the query orderedsegment.
     * @return          the trapezoid containing the leftmost endpoint.
     */
    DrawableTrapezoid* queryLeftmostFaceIntersectingSegment(const OrderedSegment& s) const;


private:
    // pointer to the root of the DAG
    DAGNode* root = nullptr;

    /**
     * @brief uniquePointers is a list containing the (pointers to the) unique nodes in the DAG.
     * This is necessary due to the fact that in a DAG several nodes may point to a leaf and this would make the recursive deallocation impossible*.
     * * if an already freed memory was freed, a runtime error would occur
     */
    std::vector<DAGNode*> uniquePointers;

    //////////////////////////////////////////////// NODE GENERATORS ////////////////////////////////////////////////
    /// \brief they create nodes and save them in the DAG.
    /// \param The paramater can be a pointer to a point, to an ordered segment or to a trapezoid.
    DAGNode* generateNode(const cg3::Point2d* const pointToStore);
    DAGNode* generateNode(const OrderedSegment* const segmentToStore);
    DAGNode* generateNode(DrawableTrapezoid* const trapezoidToStore);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief queryRec      is a private method that visit recursively the DAG. It's used both from queryFaceContaininingPoint and queryLeftmostFaceIntersectingSegment.
     * @param new_segment   it can be:
     *                          a new ordered segment just inserted,
     *                          a point disguised as a segment (both endpoints are equal to a point).
     * @param root          the current node of the DAG to visit.
     * @return              the trapezoid containing the leftmost point of the segment.
     */
    DrawableTrapezoid* queryRec(const OrderedSegment& new_segment, DAGNode* const root) const;
};



#endif // DAG_H
