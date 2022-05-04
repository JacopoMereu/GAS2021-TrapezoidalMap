#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "orderedsegment.h"
#include <cg3/geometry/point2.h>
#include <cg3/utilities/color.h>
//#include "dag.h"
//#include "dagnode.h"

class DAGNode;
class Trapezoid
{
public:
    //
    void swap(Trapezoid& other);
    Trapezoid& operator = (Trapezoid other); //note the pass-by-value

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
    void setUpperLeftNeighbor(Trapezoid* newNeighbor);
    void setUpperRightNeighbor(Trapezoid* newNeighbor);
    void setLowerLeftNeighbor(Trapezoid* newNeighbor);
    void setLowerRightNeighbor(Trapezoid* newNeighbor);
    void replaceNeighborsFromTrapezoid(Trapezoid* trapezoidToReplace, std::vector<neighborsCode> neighborsToReplace);
    Trapezoid* getUpperLeftNeighbor()  const ;
    Trapezoid* getUpperRightNeighbor() const ;
    Trapezoid* getLowerLeftNeighbor()  const ;
    Trapezoid* getLowerRightNeighbor() const ;
    // find and replace the old neighbour with the new one
    bool replaceNeighbor(Trapezoid* oldNeighbor, Trapezoid* newNeighbor);

    // setter/getter the leaf of the dag
    void setPointerToDAG(DAGNode* node);
    DAGNode* getPointerToDAG();

    // draw methods
    static double getYMin();
    static void setYMin(double newYMin);

    static double getYMax();
    static void setYMax(double newYMax);

    // y coordinates of the BB: they are useful for calculating the 4 verteces of a trapezoid
    static double yMin, yMax;
    cg3::Point2d topLeftVertex, topRightVertex, bottomLeftVertex, bottomRightVertex;
    cg3::Color color = cg3::Color(0.0, 0.0, 0.0);
    std::string cname;
    std::string ctype;
    bool isHighlighted = false;
    bool isBeingSplitted = false;

private:

    // TODO impossible to use references. Should I use pointers? :thinking:
    OrderedSegment top;     // reference to the top segment
    OrderedSegment bottom;  // reference to the bottom segment
    cg3::Point2d leftp;      // reference to the the left point
    cg3::Point2d rightp;     // reference to the top right point

    // source: https://riptutorial.com/cplusplus/example/13085/iteration-over-an-enum
    std::array<Trapezoid*, N_NEIGHBORS> neighbors = {nullptr, nullptr, nullptr, nullptr}; // array containing the adjacent trapezoids.

    // connection with the dag
    DAGNode* nodeContainer = nullptr;

    void setRandomColor();

};


//extern struct DAG::Node dagNode;

#endif // TRAPEZOID_H
