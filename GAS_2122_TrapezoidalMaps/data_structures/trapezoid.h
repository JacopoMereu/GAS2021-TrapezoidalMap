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
    //
    // A trapezoid built using general position segments has at most 4 neighbors
    static const size_t N_NEIGHBORS = 4;
    enum neighborsCode {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT};

    Trapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp);


    const OrderedSegment &getTop() const;
    void setTop(const OrderedSegment &newTop);
    const OrderedSegment &getBottom() const;
    void setBottom(const OrderedSegment &newBottom);
    const cg3::Point2d &getLeftp() const;
    void setLeftp(const cg3::Point2d &newLeftp);
    const cg3::Point2d &getRightp() const;
    void setRightp(const cg3::Point2d &newRightp);

    // setter/getter of the trapezoidal neighbors
    void setUpperLeftNeighbor(Trapezoid* const newNeighbor);
    void setUpperRightNeighbor(Trapezoid* const newNeighbor);
    void setLowerLeftNeighbor(Trapezoid* const newNeighbor);
    void setLowerRightNeighbor(Trapezoid* const newNeighbor);
    void replaceNeighborsFromTrapezoid(Trapezoid* const trapezoidToReplace, std::vector<neighborsCode> neighborsToReplace);
    Trapezoid* getUpperLeftNeighbor()  const ;
    Trapezoid* getUpperRightNeighbor() const ;
    Trapezoid* getLowerLeftNeighbor()  const ;
    Trapezoid* getLowerRightNeighbor() const ;

    // find and replace the old neighbor with the new one
    bool replaceNeighbor(Trapezoid* const oldNeighbor, Trapezoid* const newNeighbor);

    // setter/getter the leaf of the dag
    void setPointerToDAG(DAGNode* node);
    DAGNode* getPointerToDAG();

    //
    static bool canMerge(const Trapezoid& t1, const Trapezoid& t2);
    //

    bool getIsBeingSplitted() const;
    void setIsBeingSplitted(const bool newIsBeingSplitted);

protected:
    // source: https://riptutorial.com/cplusplus/example/13085/iteration-over-an-enum
    std::array<Trapezoid*, N_NEIGHBORS> neighbors = {nullptr, nullptr, nullptr, nullptr}; // array containing the adjacent trapezoids.

private:
    OrderedSegment top;
    OrderedSegment bottom;
    cg3::Point2d leftp;
    cg3::Point2d rightp;

    // connection with the dag
    DAGNode* nodeContainer = nullptr;

    bool isBeingSplitted = false;
};

#endif // TRAPEZOID_H
