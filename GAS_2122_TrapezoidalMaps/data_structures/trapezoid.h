#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "orderedsegment.h"
#include <cg3/geometry/point2.h>
//#include "dag.h"

class Trapezoid
{
public:
    //
    void swap(Trapezoid& other);
    Trapezoid& operator = (Trapezoid other); //note the pass-by-value

    //


    // A trapezoid built using general position segments has at most 4 neighbors
    static const size_t N_NEIGHBORS = 4;

    Trapezoid(OrderedSegment& t, OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp);

    OrderedSegment &getTop() const;
    void setTop(const OrderedSegment &newTop);

    OrderedSegment &getBottom() const;
    void setBottom(const OrderedSegment &newBottom);

    const cg3::Point2d &getLeftp() const;
    void setLeftp(const cg3::Point2d &newLeftp);

    const cg3::Point2d &getRightp() const;
    void setRightp(const cg3::Point2d &newRightp);

    // setter/getter of the trapezoidal neighbors
    void setUpperLeftNeighbor(Trapezoid* newNeighbor);
    void setUpperRightNeighbor(Trapezoid* newNeighbor);
    void setLowerLeftNeighbor(Trapezoid* newNeighbor);
    void setLowerRightNeighbor(Trapezoid* newNeighbor);
    Trapezoid* getUpperLeftNeighbor();
    Trapezoid* getUpperRightNeighbor();
    Trapezoid* getLowerLeftNeighbor();
    Trapezoid* getLowerRightNeighbor();

    // setter/getter the leaf of the dag
//    void setPointerToDAG(DAG::Node* node);
//    DAG::Node* getPointerToDAG();


private:
    OrderedSegment& top;     // reference to the top segment
    OrderedSegment& bottom;  // reference to the bottom segment
    cg3::Point2d leftp;      // reference to the the left point
    cg3::Point2d rightp;     // reference to the top right point

    enum neighborsCode {TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT};

    std::array<Trapezoid*, N_NEIGHBORS> neighbors = {}; // array containing the adjacent trapezoids.

    // connection with the dag
//    DAG::Node* nodeContainer = nullptr;

};

#endif // TRAPEZOID_H
