#include "trapezoid.h"

// Constructor
Trapezoid::Trapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp) : top(t), bottom(b), leftp(lp), rightp(rp)
{

}

//////////////////////// GETTER ////////////////////////
const OrderedSegment &Trapezoid::getTop() const
{
    return top;
}
const OrderedSegment &Trapezoid::getBottom() const
{
    return bottom;
}
const cg3::Point2d &Trapezoid::getLeftp() const
{
    return leftp;
}
const cg3::Point2d &Trapezoid::getRightp() const
{
    return this->rightp;
}

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

DAGNode* Trapezoid::getPointerToDAG() {
    return nodeContainer;
}

bool Trapezoid::getIsBeingSplitted() const
{
    return isBeingSplitted;
}

////////////////////////////////////////////////////////



//////////////////////// SETTER ////////////////////////
void Trapezoid::setUpperLeftNeighbor(Trapezoid* const newNeighbor) {
    neighbors[TOPLEFT] = newNeighbor;
}
void Trapezoid::setUpperRightNeighbor(Trapezoid* const newNeighbor) {
    neighbors[TOPRIGHT] = newNeighbor;
}
void Trapezoid::setLowerLeftNeighbor(Trapezoid* const newNeighbor) {
    neighbors[BOTTOMLEFT] = newNeighbor;
}
void Trapezoid::setLowerRightNeighbor(Trapezoid* const newNeighbor) {
    neighbors[BOTTOMRIGHT] = newNeighbor;
}

void Trapezoid::setPointerToDAG(DAGNode* node) {
    nodeContainer = node;
}

void Trapezoid::setIsBeingSplitted(const bool newIsBeingSplitted)
{
    isBeingSplitted = newIsBeingSplitted;
}
////////////////////////////////////////////////////////


//////////////////////// SPECIAL METHODS TO REPLACE NEIGHBORS ////////////////////////////////////////////////////////////////////////
bool Trapezoid::replaceNeighbor(Trapezoid* const oldNeighbor, Trapezoid* const newNeighbor) {
    bool hasReplaced = false;
    // source: https://riptutorial.com/cplusplus/example/13085/iteration-over-an-enum
    for (neighborsCode i = TOPLEFT; i <= BOTTOMRIGHT; i = neighborsCode(i + 1))
    {
        if(neighbors[i] == oldNeighbor) {
            neighbors[i] = newNeighbor;
            hasReplaced = true;
            break;
        }
    }
    return hasReplaced;
}

void Trapezoid::replaceNeighborsFromTrapezoid(Trapezoid* const trapezoidToReplace, std::vector<neighborsCode> neighborsToReplace) {
    for(auto code : neighborsToReplace) {
        this->neighbors[code] = trapezoidToReplace->neighbors[code];
        if(trapezoidToReplace->neighbors[code] != nullptr) {
            trapezoidToReplace->neighbors[code]->replaceNeighbor(trapezoidToReplace, this);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool Trapezoid::canMerge(const Trapezoid& t1, const Trapezoid& t2) {
    return  t1.getTop() == t2.getTop()
            && t1.getBottom() == t2.getBottom();
}



