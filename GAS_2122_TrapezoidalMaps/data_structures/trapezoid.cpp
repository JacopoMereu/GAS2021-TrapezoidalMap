#include "trapezoid.h"
#include <cg3/geometry/intersections2.h>

double Trapezoid::yMin = -1;
double Trapezoid::yMax = -1;

Trapezoid::Trapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp) : top(t), bottom(b), leftp(lp), rightp(rp)
{
    //  PRE-COMPUTING THE 4 VERTECES THAT MADE UP THE TRAPEZOID
    cg3::Segment2d leftVerticalLine = cg3::Segment2d(
                cg3::Point2d(lp.x(), Trapezoid::getYMax()),
                cg3::Point2d(lp.x(), Trapezoid::getYMin())
                );
    cg3::Segment2d rightVerticalLine = cg3::Segment2d(
                    cg3::Point2d(rp.x(), Trapezoid::getYMax()),
                    cg3::Point2d(rp.x(), Trapezoid::getYMin())
                    );
    char code;
    cg3::checkSegmentIntersection2(leftVerticalLine,  t, code, cg3::CG3_EPSILON, topLeftVertex);
    assert(code == 'v' || code == '1');
    cg3::checkSegmentIntersection2(leftVerticalLine,  b, code, cg3::CG3_EPSILON, bottomLeftVertex);
    assert(code == 'v' || code == '1');
    cg3::checkSegmentIntersection2(rightVerticalLine, t, code, cg3::CG3_EPSILON, topRightVertex);
    assert(code == 'v' || code == '1');
    cg3::checkSegmentIntersection2(rightVerticalLine, t, code, cg3::CG3_EPSILON, bottomRightVertex);
    assert(code == 'v' || code == '1');

}

const OrderedSegment &Trapezoid::getTop() const
{
    return top;
}

void Trapezoid::setTop(const OrderedSegment &newTop)
{
    top = newTop;
}

const OrderedSegment &Trapezoid::getBottom() const
{
    return bottom;
}

void Trapezoid::setBottom(const OrderedSegment &newBottom)
{
    bottom = newBottom;
}

const cg3::Point2d &Trapezoid::getLeftp() const
{
    return leftp;
}

void Trapezoid::setLeftp(const cg3::Point2d &newLeftp)
{
    leftp = newLeftp;
}

const cg3::Point2d &Trapezoid::getRightp() const
{
    return this->rightp;
}

void Trapezoid::setRightp(const cg3::Point2d &newRightp)
{
    this->rightp= newRightp;
}


// setter/getter of the trapezoidal neighbors
void Trapezoid::setUpperLeftNeighbor(Trapezoid* newNeighbor) {
    neighbors[TOPLEFT] = newNeighbor;      
}
void Trapezoid::setUpperRightNeighbor(Trapezoid* newNeighbor) {
    neighbors[TOPRIGHT] = newNeighbor;
}
void Trapezoid::setLowerLeftNeighbor(Trapezoid* newNeighbor) {
    neighbors[BOTTOMLEFT] = newNeighbor;
}
void Trapezoid::setLowerRightNeighbor(Trapezoid* newNeighbor) {
    neighbors[BOTTOMRIGHT] = newNeighbor;
}
//
void Trapezoid::replaceNeighborsFromTrapezoid(Trapezoid* trapezoidToReplace, std::vector<neighborsCode> neighborsToReplace) {
    for(auto code : neighborsToReplace) {
        this->neighbors[code] = trapezoidToReplace->neighbors[code];
        if(trapezoidToReplace->neighbors[code] != nullptr) {
            trapezoidToReplace->neighbors[code]->replaceNeighbor(trapezoidToReplace, this);
        }
    }
}
//
Trapezoid* Trapezoid::getUpperLeftNeighbor() const {
    return neighbors[TOPLEFT];
}
Trapezoid* Trapezoid::getUpperRightNeighbor() const {
    return neighbors[TOPRIGHT];
}
Trapezoid* Trapezoid::getLowerLeftNeighbor() const {
    return neighbors[BOTTOMLEFT];
}
Trapezoid* Trapezoid::getLowerRightNeighbor() const {
    return neighbors[BOTTOMRIGHT];
}
bool Trapezoid::replaceNeighbor(Trapezoid* oldNeighbor, Trapezoid* newNeighbor) {
    bool hasReplaced = false;
    //TODO da ottimizare il senso di ricerca
    for (neighborsCode i = TOPLEFT; i < BOTTOMRIGHT; i = neighborsCode(i + 1))
    {
        if(neighbors[i] == oldNeighbor) {
            neighbors[i] = newNeighbor;
            hasReplaced = true;
            break;
        }
    }
    return hasReplaced;
}


//
void Trapezoid::setPointerToDAG(DAGNode* node) {
    nodeContainer = node;
}

DAGNode* Trapezoid::getPointerToDAG() {
    return nodeContainer;
}

double Trapezoid::getYMin()
{
    return Trapezoid::yMin;
}

void Trapezoid::setYMin(double newYMin)
{
    Trapezoid::yMin = newYMin;
}

double Trapezoid::getYMax()
{
    return Trapezoid::yMax;
}

void Trapezoid::setYMax(double newYMax)
{
    Trapezoid::yMax = newYMax;
}
//

//
void getDrawablePoints(cg3::Point2d topleft, cg3::Point2d topright, cg3::Point2d bottomleft, cg3::Point2d bottomright) {

//    cg3::checkSegmentIntersection2();
}
//

void Trapezoid::swap(Trapezoid& other)
{
//    using std::swap; //good practice
    std::swap(top, other.top); //and all the other members
    std::swap(bottom, other.bottom); //and all the other members
    std::swap(leftp, other.leftp); //and all the other members
    std::swap(rightp, other.rightp); //and all the other members

}
Trapezoid& Trapezoid::operator = (Trapezoid other)
{
    this->swap(other);
    return *this;
}

