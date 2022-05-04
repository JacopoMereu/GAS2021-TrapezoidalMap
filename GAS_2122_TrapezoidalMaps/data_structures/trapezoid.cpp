#include "trapezoid.h"
#include <cg3/geometry/intersections2.h>
#include <random>
double Trapezoid::yMin = -1;
double Trapezoid::yMax = -1;

//TODO Temporary
// source: https: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
void Trapezoid::setRandomColor() {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    assert(this->color!=nullptr);
    if (dis(gen)<0.2) {
        // red
        this->color.setRedF(1.0);
        this->color.setGreenF(0);
        this->color.setBlueF(0);

        cname="RED";
    } else  if (dis(gen)<0.2) {
        // green
        this->color.setRedF(0);
        this->color.setGreenF(1.0);
        this->color.setBlueF(0);

                cname="GREEN";
    } else  if (dis(gen)<0.4) {
        // blue
        this->color.setRedF(0);
        this->color.setGreenF(0);
        this->color.setBlueF(1);

                cname="BLUE";
    } else  if (dis(gen)<0.6) {
        // purple
        this->color.setRedF(0.51);
        this->color.setGreenF(0.51);
        this->color.setBlueF(0);

                cname="PURPLE";
    } else  if (dis(gen)<0.8) {
        // white
        this->color.setRedF(1);
        this->color.setGreenF(1);
        this->color.setBlueF(1);

                cname="WHITE";
    } else  {
        // black
        this->color.setRedF(0);
        this->color.setGreenF(0);
        this->color.setBlueF(0);

                cname="BLACK";
    }
    /*this->color.setRedF(dis(gen));
    this->color.setBlueF(dis(gen));
    this->color.setGreenF(dis(gen));*/
}
Trapezoid::Trapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp) : top(t), bottom(b), leftp(lp), rightp(rp)
{
    setRandomColor();
//    for(int i = 0; i < N_NEIGHBORS;)
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
    double thres = 1; // cg3::CG3_EPSILON
/*    cg3::checkSegmentIntersection2(leftVerticalLine,  t, code, thres, topLeftVertex);
    assert(code == 'v' || code == '1');

    cg3::checkSegmentIntersection2(rightVerticalLine, t, code, thres, topRightVertex);
    assert(code == 'v' || code == '1');

    cg3::checkSegmentIntersection2(rightVerticalLine, b, code, thres, bottomRightVertex);
    assert(code == 'v' || code== '1');

   cg3::checkSegmentIntersection2(leftVerticalLine,  b, code, thres, bottomLeftVertex);
        assert(code == 'v' || code == '1');
*/

    if(lp == b.getLeftmost()) {
        this->bottomLeftVertex = lp;
        cg3::checkSegmentIntersection2(leftVerticalLine,  t, code, thres, topLeftVertex);
    }
    else if (lp == t.getLeftmost()) {
        this->topLeftVertex = lp;
        cg3::checkSegmentIntersection2(leftVerticalLine,  b, code, thres, bottomLeftVertex);
        assert(code == 'v' || code == '1');
    } else {
        cg3::checkSegmentIntersection2(leftVerticalLine,  t, code, thres, topLeftVertex);
        assert(code == 'v' || code == '1');
        cg3::checkSegmentIntersection2(leftVerticalLine,  b, code, thres, bottomLeftVertex);
        assert(code == 'v' || code == '1');
    }

    //

    if(rp == b.getRightmost()) {
        this->bottomRightVertex = rp;
        cg3::checkSegmentIntersection2(rightVerticalLine,  t, code, thres, topRightVertex);
    }
    else if (rp == t.getRightmost()) {
        this->topRightVertex = rp;
        cg3::checkSegmentIntersection2(rightVerticalLine,  b, code, thres, bottomRightVertex);
        assert(code == 'v' || code == '1');

    } else {
        cg3::checkSegmentIntersection2(rightVerticalLine, t, code, thres, topRightVertex);
        assert(code == 'v' || code == '1');

        cg3::checkSegmentIntersection2(rightVerticalLine, b, code, thres, bottomRightVertex);
        assert(code == 'v' || code== '1');
    }


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

