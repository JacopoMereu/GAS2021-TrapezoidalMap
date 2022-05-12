#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "orderedsegment.h"
#include <cg3/geometry/point2.h>
#include <cg3/utilities/color.h>
#include "cg3/geometry/bounding_box2.h"

class DAGNode;
class Trapezoid
{
public:
    // A trapezoid is built using segments in general position, so it has at most 4 neighbors
    static const size_t N_NEIGHBORS = 4;

    // The neighborsCode enum represents the 4 types of neighbors: topright, topleft, bottomleft, bottomright
    enum neighborsCode {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT};

    /**
     * @brief Trapezoid Constructor of a trapezoid
     * @param t         the segment above
     * @param b         the segment below
     * @param lp        the left point
     * @param rp        the right point
     */
    Trapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp);



    //////////////////////// GETTER ////////////////////////
    // returns a reference to the top (orderedsegment) of this trapezoid
    const OrderedSegment &getTop() const;
    // returns a reference to the bottom (orderedsegment) of this trapezoid
    const OrderedSegment &getBottom() const;
    // returns a reference to the left point (point) of this trapezoid
    const cg3::Point2d &getLeftp() const;
    // returns a reference to the right point (point) of this trapezoid
    const cg3::Point2d &getRightp() const;

    // Getters of the pointers to the neighbors of this trapezoid. If a neighbor didn't exist, nullptr would be returned.
    Trapezoid* getUpperLeftNeighbor()  const ;
    Trapezoid* getUpperRightNeighbor() const ;
    Trapezoid* getLowerLeftNeighbor()  const ;
    Trapezoid* getLowerRightNeighbor() const ;

    // returns the pointer to the leaf in the DAG pointing this trapezoid. If the node didn't exist, nullptr would be returned.
    DAGNode* getPointerToDAG();

    // returns true if the trapezoid is being split by a new segment, false otherwise.
    bool getIsBeingSplitted() const;
    ////////////////////////////////////////////////////////



    //////////////////////// SETTER ////////////////////////

    // Set the pointer to a certain neighbor using the pointer given in input
    void setUpperLeftNeighbor(Trapezoid* const newNeighbor);
    void setUpperRightNeighbor(Trapezoid* const newNeighbor);
    void setLowerLeftNeighbor(Trapezoid* const newNeighbor);
    void setLowerRightNeighbor(Trapezoid* const newNeighbor);

    // Set the pointer to the leaf (in the DAG) pointing to this trapezoid
    void setPointerToDAG(DAGNode* node);

    // Set the flag isBeingSplitted with the boolean given in input
    void setIsBeingSplitted(const bool newIsBeingSplitted);
    ////////////////////////////////////////////////////////


    //////////////////////// SPECIAL METHODS TO REPLACE NEIGHBORS ////////////////////////////////////////////////////////////////////////

    /**
     * @brief replaceNeighbor       seeks if there's a particular trapezoid as neighbor, if yes, substitute it with another trapezoid.
     * @param oldNeighbor           the old neighbor to seek and, if found, to substitute.
     * @param newNeighbor           the new neighbor.
     * @return                      true if the old neighbor has been found and replaced, false otherwise.
     */
    bool replaceNeighbor(Trapezoid* const oldNeighbor, Trapezoid* const newNeighbor);

    /**
     * @brief replaceNeighborsFromTrapezoid     "Steal" all the neighbors, specified in a list, from a given trapezoid. For each neighbor stolen, if it's not null, replace its old neighbor with this.
     * @param trapezoidToReplace                The trapezoid from which the neighbors will be "stolen".
     * @param neighborsToReplace                The list of neighbors to steal (UPPER_LEFT, LOWER_LEFT, ...)
     */
    void replaceNeighborsFromTrapezoid(Trapezoid* const trapezoidToReplace, std::vector<neighborsCode> neighborsToReplace);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief canMerge      Check if two trapezoids can be merged (i.e. they have the same top and bottom segments)
     * @param t1            A reference to the first trapezoid.
     * @param t2            A reference to the second trapezoid.
     * @return              true if the two trapezoids can be merged, false otherwise.
     */
    static bool canMerge(const Trapezoid& t1, const Trapezoid& t2);

protected:
    // array containing the 4 adjacent trapezoids.
    std::array<Trapezoid*, N_NEIGHBORS> neighbors = {nullptr, nullptr, nullptr, nullptr};

private:
    // Objects representing a trapezoid: top segment, bottom segment, left point, right point.
    OrderedSegment top;
    OrderedSegment bottom;
    cg3::Point2d leftp;
    cg3::Point2d rightp;

    // Pointer to the leaf pointing this trapezoid
    DAGNode* nodeContainer = nullptr;

    // flag that checks if this trapezoids is being split by a new segment
    bool isBeingSplitted = false;
};

#endif // TRAPEZOID_H
