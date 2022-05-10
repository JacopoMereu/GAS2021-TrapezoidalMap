#include "trapezoid.h"

// Constructor
Trapezoid::Trapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp) : top(t), bottom(b), leftp(lp), rightp(rp)
{

}

// setter/getter of top/bottom/left point/ right point
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
// Replace neighbors
void Trapezoid::replaceNeighborsFromTrapezoid(Trapezoid* trapezoidToReplace, std::vector<neighborsCode> neighborsToReplace) {
    for(auto code : neighborsToReplace) {
        this->neighbors[code] = trapezoidToReplace->neighbors[code];
        if(trapezoidToReplace->neighbors[code] != nullptr) {
            trapezoidToReplace->neighbors[code]->replaceNeighbor(trapezoidToReplace, this);
        }
    }
}
bool Trapezoid::replaceNeighbor(Trapezoid* oldNeighbor, Trapezoid* newNeighbor) {
    bool hasReplaced = false;
    //TODO da ottimizare il senso di ricerca
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
// Setter/Getter pointer to the container node
void Trapezoid::setPointerToDAG(DAGNode* node) {
    nodeContainer = node;
}
DAGNode* Trapezoid::getPointerToDAG() {
    return nodeContainer;
}
// Merge Trapezoid
bool Trapezoid::canMerge(const Trapezoid& t1, const Trapezoid& t2) {
    return  t1.getTop() == t2.getTop()
            && t1.getBottom() == t2.getBottom();
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

