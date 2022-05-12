#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "dag.h"
#include "drawables/drawabletrapezoid.h"
#include "cg3/geometry/bounding_box2.h"

class TrapezoidalMap
{
public:
    // Constructor
    TrapezoidalMap();
    // Destructor
    ~TrapezoidalMap();

    /**
     * @brief initialize    initializes the trapezoidal map (and the DAG inside it) creating the first trapezoid.
     *                      This last represents the BoundingBox that contains both the trapezoidal map and the DAG.
     * @param B             The bounding box that encloses the trapezoidal mal (and the DAG).
     */
    virtual void initialize(const cg3::BoundingBox2& B);

    /**
     * @brief addSegment        inserts a new segment in the trapezoidal map.
     * First of all, the trapezoids intersected by the new segment will be found.
     * Secondly, these will be split in multiple parts. If two part have the same top and bottom, they will be merged.
     * The new faces will be inserted in the trapezoidal map, thereafter the DAG will be updated (every leaf pointining to an a split face will be replaced by a new subtree).
     * Finally, the old faces split will be removed from the trapezoidal map.
     * @param segment           the new segment.
     */
    void addSegment(const cg3::Segment2d& segment);

    /**
     * @brief pointLocation     query a point in the trapezoidal map.
     * @param pointToQuery      the query point.
     * @return                  the (drawable) trapezoid containing the query point.
     */
    DrawableTrapezoid* pointLocation(const cg3::Point2d& pointToQuery) const;


    // the trapezoidal map and the DAG inside it. All the memory dynamically allocated will be freed.
    void clear();

    // the trapezoidal map (and the DAG inside it). The data structures are cleared before and initialized again after.
    void reset();

protected:
    // list of trapezoids in the map
    std::vector<DrawableTrapezoid*> T;

    // get the bounding box
    const cg3::BoundingBox2 &getBoundingBox() const;

private:
    // list of the segments inserted into the map
    std::vector<OrderedSegment*> segments;

    // DAG is hidden inside the trapezoidal map
    DAG D;

    // The bounding box used to initialize the trapezoidal map.
    cg3::BoundingBox2 B;

    // set the bounding box containing the trapezoidal map
    void setBoundingBox(const cg3::BoundingBox2 &newB);

    /**
     * @brief followSegment                     searches for all the trapezoid intersecting a given segment.
     * @param s                                 the query segment.
     * @param [out] facesIntersectingSegment    the faces intersecting the segment will be saved in this list
     */
    void followSegment(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& facesIntersectingSegment) const;

    ///////////////////////////////////////// Split ////////////////////////////////////////////
    /**
     * @brief split                 split all the trapezoids intersected by a segment. The new faces will be added into the data structures, while the old ones will be deleted.
     * @param s                     the segment splitting the trapezoids
     * @param intersectingFaces     the list of trapezoids intersecting the segment
     */
    void split(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);

    /**
     * @brief splitSingularTrapezoid        contains the logic for splitting a trapezoid containing a segment.
     *                                      The new faces will be added into the data structures, while the old one will NOT be deleted here.
     * @param s                             the (ordered) segment.
     * @param faceToSplit                   the trapezoid to split.
     */
    void splitSingularTrapezoid(const OrderedSegment& s, DrawableTrapezoid* faceToSplit);

    /**
     * @brief splitMultipleTrapezoid        contains the logic for splitting a list of trapezoids intersecting a segment.
     *                                      The new faces will be added into the data structures, while the old one will NOT be deleted here.
     * @param s                             the (ordered) segment.
     * @param intersectingFaces             the list of trapezoids to split.
     */
    void splitMultipleTrapezoid(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces);

    /**
     * @brief stepMerging       contains the logic for checking and merging (if possible) the trapezoids contained in a list. The new trapezoids will be added into the trapezoidal map (but not into the DAG yet).
     * @param start             the position of the first trapezoid (included)
     * @param end               the position of the last trapezoid  (excluded)
     * @param list              the list containing the trapezoids to merge. At the end of the function it will contain the trapezoids merged.
     *                          If two or more trapezoids are merged into a singular one, the new one will replace the old ones in each position of the list, therefore the size of the list will remain invariant.
     *                          e.g. Let's say B, C and D are merged into G, here's what will happen: [A, B, C, D, E, F] => [A, G, G, G, F].
     */
    void stepMerging(size_t start, size_t end, std::vector<DrawableTrapezoid*>& list);
    /////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief addTrapezoidToMap     adds a (drawable) trapezoid to the trapezoidal map (but not into the DAG). Before doing this, the trapezoid graphics is calculated.
     * @param trapezoidToAdd        the trapezoid to insert
     */
    void addTrapezoidToMap(DrawableTrapezoid* trapezoidToAdd);

    /**
     * @brief deleteTrapezoidFromMap removes a trapezoid from the trapezoidal map (but not from the DAG) and frees the dynamically allocated memory.
     * @param trapezoidToDelete     the trapezoid to delete.
     */
    void deleteTrapezoidFromMap(DrawableTrapezoid* trapezoidToDelete);
};

#endif // TRAPEZOIDALMAP_H
