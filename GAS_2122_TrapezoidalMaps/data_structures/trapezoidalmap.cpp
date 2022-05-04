#include "trapezoidalmap.h"

#include "cg3/geometry/utils2.h"
// ----------------------- PUBLIC SECTION -----------------------
TrapezoidalMap::~TrapezoidalMap() {
    // deleting the dag
    D.clear();

    // deletig the faces
   for (auto iterable_face=T.begin(); iterable_face != T.end(); iterable_face++)
        if(*iterable_face)
            delete *iterable_face;
    T.clear();
    T.shrink_to_fit();
    // delete the segments (and points)
    for (auto iterable_segment=segments.begin(); iterable_segment != segments.end(); iterable_segment++)
        if(*iterable_segment)
            delete *iterable_segment;
    segments.clear();
    segments.shrink_to_fit();
}

TrapezoidalMap::TrapezoidalMap() {}


void TrapezoidalMap::initialize(const cg3::BoundingBox2& B)
{
    // Saving the y-coordinates in the Trapezoid class
    setBoundingBox(B);
    auto yMin = B.min().y();
    auto yMax = B.max().y();
    Trapezoid::setYMax(yMax);
    Trapezoid::setYMin(yMin);

    // Create a trapezoid from the bounding box
    //Trapezoid* boundingbox_trapezoid = Trapezoid::generateTrapezoid(B);
    auto topleft     = cg3::Point2d(B.min().x(), yMax);
    auto topright    = cg3::Point2d(B.max());
    auto bottomleft  = cg3::Point2d(B.min());
    auto bottomright = cg3::Point2d(B.max().x(), yMin);
    OrderedSegment* top = new OrderedSegment(topleft, topright);
    OrderedSegment* bottom = new OrderedSegment(bottomleft, bottomright);
    Trapezoid* boundingbox_trapezoid = new Trapezoid(*top, *bottom, bottomleft, topright);
    //egments = std::vector<OrderedSegment*>();
    segments.push_back(top);
    segments.push_back(bottom);

    // Initialize the trapezoidal map structure T and search structure D
    //T = std::vector<Trapezoid*>();
    T.push_back(boundingbox_trapezoid);
    D.initialize(boundingbox_trapezoid);
}


void TrapezoidalMap::addSegment(const cg3::Segment2d& segment) {
    // Find the faces in the trapezoidal map T that intersect the segment
    // sorted from left to right
        ///TODO D.tmp();
    OrderedSegment* orderedSegment = new OrderedSegment(segment);
    segments.push_back(orderedSegment);

    auto facesIntersected = std::vector<Trapezoid*>();
    followSegment(*orderedSegment , facesIntersected);
    // Split those faces and update the map/dag with the new faces
    split(*orderedSegment , facesIntersected);
}

Trapezoid* TrapezoidalMap::pointLocation(const cg3::Point2d& pointToQuery) {
    return D.query(pointToQuery);
}

void TrapezoidalMap::clear() {    
   // Cleaning the data dynamically instantiated
   this->~TrapezoidalMap();

    // Re-initializing the data structures
    this->initialize(this->getBoundingBox());

}
// ----------------------- END PUBLIC SECTION -----------------------

// ---- PROTECTED
const cg3::BoundingBox2 &TrapezoidalMap::getBoundingBox() const
{
    return B;
}

void TrapezoidalMap::setBoundingBox(const cg3::BoundingBox2 &newB)
{
    B = newB;
}

// -------------------


// ----------------------- PRIVATE SECTION -----------------------
void TrapezoidalMap::followSegment(OrderedSegment& s, std::vector<Trapezoid*>& facesIntersectingSegment) {
    // 1. Let p and q be the left and right endpoint of s.
    auto p = s.getLeftmost();
    auto q = s.getRightmost();
    // 2. Search with p in the search structure D to find d0.
    Trapezoid* currentFace = D.query(p);

    facesIntersectingSegment.push_back(currentFace);
    currentFace->isBeingSplitted = true;
    //  while q lies to the right of rightp(dj)
    while(currentFace != nullptr && q.x() > currentFace->getRightp().x()) {
        ///Position pos = OrientationUtility::getPointPositionRespectToLine(tmp->getRightp(), s);
        // if rightp(dj) lies above the segment => go on the LowRight neighbor
        ///if(pos == above) {
        if(cg3::isPointAtLeft(s.getLeftmost(), s.getRightmost(), currentFace->getRightp())) {
            currentFace = currentFace->getLowerRightNeighbor();
        }
        // else if rightp(dj) lies below the segment => go on the TopRight neighbor
        ///else if (pos == below) {
        else if(cg3::isPointAtRight(s.getLeftmost(), s.getRightmost(), currentFace->getRightp())) {
            currentFace = currentFace->getUpperRightNeighbor();
        }
        // This case should be impossible ?
        else {
            assert(false);
        }

        //
        if(currentFace != nullptr) {
            facesIntersectingSegment.push_back(currentFace);
            currentFace->isBeingSplitted = true;
        }
    }
}


void TrapezoidalMap::split(OrderedSegment& s, std::vector<Trapezoid*>& intersectingFaces) {
// it's better to pass the whole list to the function instead of a single trapezoid (because there are several scenarios to handle...)
    const size_t N_FACES = intersectingFaces.size();
    /* EASIEST CASE */
    if(N_FACES== 1) {
        Trapezoid* oldFace = intersectingFaces.front();

        // left
        Trapezoid* leftNewFace = new Trapezoid(oldFace->getTop(), oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());
        // top
        Trapezoid* topNewFace = new Trapezoid(oldFace->getTop(), s, s.getLeftmost(), s.getRightmost());
        // right
        Trapezoid* rightNewFace = new Trapezoid(oldFace->getTop(), oldFace->getBottom(), s.getRightmost(), oldFace->getRightp());
        // bottom
        Trapezoid* bottomNewFace = new Trapezoid(s, oldFace->getBottom(), s.getLeftmost(), s.getRightmost());

        // setting adjacencies for:
        // left
        leftNewFace->replaceNeighborsFromTrapezoid(oldFace, {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
        leftNewFace->setUpperRightNeighbor(topNewFace);
        leftNewFace->setLowerRightNeighbor(bottomNewFace);

        // top
        topNewFace->setUpperLeftNeighbor(leftNewFace);
        topNewFace->setUpperRightNeighbor(rightNewFace);

        // right
        rightNewFace->setUpperLeftNeighbor(topNewFace);
        rightNewFace->setLowerLeftNeighbor(bottomNewFace);
        rightNewFace->replaceNeighborsFromTrapezoid(oldFace, {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

        // bottom
        bottomNewFace->setLowerLeftNeighbor(leftNewFace);
        bottomNewFace->setLowerRightNeighbor(rightNewFace);

        // Add the new faces on the trapezoidal map
        T.push_back(leftNewFace);
        T.push_back(topNewFace);
        T.push_back(bottomNewFace);
        T.push_back(rightNewFace);
        leftNewFace->ctype = "left simple";
        topNewFace->ctype = "top simple";
        bottomNewFace->ctype = "bottom simple";
        rightNewFace->ctype = "right simple";

        // Upgrade the DAG
        D.replaceNodeWithSubtree(oldFace->getPointerToDAG(), s, leftNewFace, topNewFace, bottomNewFace, rightNewFace);

        // Remove the old one from T
        auto tmp = intersectingFaces.front();
        intersectingFaces.clear();
        T.erase(std::remove(T.begin(), T.end(), tmp), T.end());
        if(tmp)
            delete tmp;
        assert(intersectingFaces.empty() == true);
    }

    /* HARDEST CASE */
    else {
        Trapezoid* firstFace, *lastFace;
        std::vector<Trapezoid*> aboveSegmentNewFaces(N_FACES+2);
        std::vector<Trapezoid*> belowSegmentNewFaces(N_FACES+2);
        // source: https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
        std::vector<std::vector<Trapezoid*>> finalNewFaces(2, std::vector <Trapezoid*> (N_FACES, nullptr));

        /* SPLIT THE OLD FACES */
        for(size_t i = 0; i<intersectingFaces.size(); i++){
            Trapezoid* oldFace = intersectingFaces.at(i);
            // first face
            if(intersectingFaces.at(i)==intersectingFaces.front()){
                /* split it into three parts: leftNewFace, topNewFace, bottomNewFace*/
                // left
                firstFace = new Trapezoid(oldFace->getTop(), oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());
                // top
                Trapezoid* topNewFace = new Trapezoid(oldFace->getTop(), s, s.getLeftmost(), oldFace->getRightp());
                // bottom
                Trapezoid* bottomNewFace = new Trapezoid(s, oldFace->getBottom(), s.getLeftmost(), oldFace->getRightp());

                //
                firstFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                firstFace->setUpperRightNeighbor(topNewFace);
                firstFace->setLowerRightNeighbor(bottomNewFace);
                //
                aboveSegmentNewFaces.at(i)=  firstFace;
                belowSegmentNewFaces.at(i) = firstFace;

                aboveSegmentNewFaces.at(i+1) = topNewFace;
                belowSegmentNewFaces.at(i+1) = bottomNewFace;
            }
            // last face
            else if(intersectingFaces.at(i)==intersectingFaces.back()) {
                /* split it into three parts: topNewFace, bottomNewFace, rightNewFace*/
                // right
                lastFace = new Trapezoid(oldFace->getTop(), oldFace->getBottom(), s.getRightmost(), oldFace->getRightp());

                // top
                Trapezoid* topNewFace = new Trapezoid(oldFace->getTop(), s, oldFace->getLeftp(), s.getRightmost());
                // bottom
                Trapezoid* bottomNewFace = new Trapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), s.getRightmost());

                //
                lastFace->setUpperLeftNeighbor(topNewFace);
                lastFace->setLowerLeftNeighbor(bottomNewFace);
                lastFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});
                //
                aboveSegmentNewFaces.at(i+1) = topNewFace;
                belowSegmentNewFaces.at(i+1) = bottomNewFace;

                aboveSegmentNewFaces.at(i+2) = lastFace;
                belowSegmentNewFaces.at(i+2) = lastFace;


            }
            // faces 1...n-1
            else {
                /* split it into two parts: topNewFace, bottomNewFace*/
                // top
                Trapezoid* topNewFace = new Trapezoid(oldFace->getTop(), s, oldFace->getLeftp(), oldFace->getRightp());
                // bottom
                Trapezoid* bottomNewFace = new Trapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), oldFace->getRightp());

                //
                aboveSegmentNewFaces.at(i+1) = topNewFace;
                belowSegmentNewFaces.at(i+1) = bottomNewFace;
            }
        }

        // Set the neighbors of the new faces
        for(size_t i = 1; i < aboveSegmentNewFaces.size()-1; i++) {
            /* ABOVE */
            Trapezoid* tmpTopFace = aboveSegmentNewFaces.at(i);

            if(i==1) {
                if(intersectingFaces.at(i-1)->getUpperRightNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getUpperRightNeighbor()->isBeingSplitted) {
                    tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::TOPRIGHT});
                }
                tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces.at(i+1));
                tmpTopFace->setLowerLeftNeighbor(nullptr); // can be deleted
                tmpTopFace->setUpperLeftNeighbor(aboveSegmentNewFaces.at(i-1)); // == left face

            } else if (i==aboveSegmentNewFaces.size()-2) {
                tmpTopFace->setUpperRightNeighbor(aboveSegmentNewFaces.at(i+1));
                tmpTopFace->setLowerRightNeighbor(nullptr); // can be deleted
                tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces.at(i-1));
                if(intersectingFaces.at(i-1)->getUpperLeftNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getUpperLeftNeighbor()->isBeingSplitted) {
                    tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::TOPLEFT});
                }
            } else {
                if(intersectingFaces.at(i-1)->getUpperRightNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getUpperRightNeighbor()->isBeingSplitted) {
                    tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::TOPRIGHT});
                }
                tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces.at(i+1));
                tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces.at(i-1));
                if(intersectingFaces.at(i-1)->getUpperLeftNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getUpperLeftNeighbor()->isBeingSplitted) {
                    tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::TOPLEFT});
                }
            }
            /* BELOW */
            Trapezoid* tmpBottomFace = belowSegmentNewFaces.at(i);
            if(i==1) {
                tmpBottomFace->setUpperRightNeighbor(belowSegmentNewFaces.at(i+1));
                if(intersectingFaces.at(i-1)->getLowerRightNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getLowerRightNeighbor()->isBeingSplitted) {
                    tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::BOTTOMRIGHT});
                }
                tmpBottomFace->setLowerLeftNeighbor(belowSegmentNewFaces.at(i-1)); // == left face
                tmpBottomFace->setUpperLeftNeighbor(nullptr); // can be deleted

            } else if (i==belowSegmentNewFaces.size()-2) {
                tmpBottomFace->setUpperRightNeighbor(nullptr);
                tmpBottomFace->setLowerRightNeighbor(belowSegmentNewFaces.at(i+1));
                if(intersectingFaces.at(i-1)->getLowerLeftNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getLowerLeftNeighbor()->isBeingSplitted) {
                    tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::BOTTOMLEFT});
                }
                tmpBottomFace->setUpperLeftNeighbor(belowSegmentNewFaces.at(i-1));
            } else {
                tmpBottomFace->setUpperRightNeighbor(belowSegmentNewFaces.at(i+1));
                if(intersectingFaces.at(i-1)->getLowerRightNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getLowerRightNeighbor()->isBeingSplitted) {
                    tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::BOTTOMRIGHT});
                }
                if(intersectingFaces.at(i-1)->getLowerLeftNeighbor()!=nullptr && !intersectingFaces.at(i-1)->getLowerLeftNeighbor()->isBeingSplitted) {
                    tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::BOTTOMLEFT});
                }
                tmpBottomFace->setUpperLeftNeighbor(belowSegmentNewFaces.at(i-1));
            }
        }

        // Merging the eventually adjacent faces with same bottom and top
        /* ABOVE */
        T.push_back(firstFace);
        firstFace->ctype = "left-first complex";

        for(size_t i = 1; i < N_FACES+1; i++) {
            size_t j = i;
            // TODO == operator of OrderedSegment should be ok since it was inherited by cg3::Segment(?)
            while(j<N_FACES
                  && aboveSegmentNewFaces.at(j)->getTop()    == aboveSegmentNewFaces.at(j+1)->getTop()
                  && aboveSegmentNewFaces.at(j)->getBottom() == aboveSegmentNewFaces.at(j+1)->getBottom())
                j++;

            // If I found faces to merge
            if(j != i) {
                // Create the new face
                Trapezoid* mergedFace = new Trapezoid(aboveSegmentNewFaces.at(i)->getTop(), aboveSegmentNewFaces.at(i)->getBottom(), aboveSegmentNewFaces.at(i)->getLeftp(), aboveSegmentNewFaces.at(j)->getRightp());
                // Set the neighbors
                mergedFace->replaceNeighborsFromTrapezoid(aboveSegmentNewFaces.at(i), {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                mergedFace->replaceNeighborsFromTrapezoid(aboveSegmentNewFaces.at(j), {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

                for(auto k=i; k<=j; k++) {
                    if(finalNewFaces[0].at(k-1))
                        delete finalNewFaces[0].at(k-1);
                    finalNewFaces[0].at(k-1) = mergedFace;
                }
                // Jump to the face after Fj
                i = j;
                T.push_back(mergedFace);
                mergedFace->ctype = "top-merged complex";

            }
            // Otherwise
            else {
                finalNewFaces[0].at(i-1) = aboveSegmentNewFaces.at(i);
                T.push_back(finalNewFaces[0].at(i-1));
                finalNewFaces[0].at(i-1)->ctype = "top-NON-merged complex";

            }
        }

        /* BELOW */
        for(size_t i = 1; i < N_FACES+1; i++) {
            size_t j = i;
            // TODO == operator of OrderedSegment should be ok since it was inherited by cg3::Segment(?)
            while(j<N_FACES
                  && (belowSegmentNewFaces.at(j))->getTop()    == belowSegmentNewFaces.at(j+1)->getTop()
                  && belowSegmentNewFaces.at(j)->getBottom() == belowSegmentNewFaces.at(j+1)->getBottom())
                j++;

            // If I found faces to merge
            if(j != i) {
                // Create the new face
                Trapezoid* mergedFace = new Trapezoid(belowSegmentNewFaces.at(i)->getTop(), belowSegmentNewFaces.at(i)->getBottom(), belowSegmentNewFaces.at(i)->getLeftp(), belowSegmentNewFaces.at(j)->getRightp());
                // Set the neighbors
                mergedFace->replaceNeighborsFromTrapezoid(belowSegmentNewFaces.at(i), {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                mergedFace->replaceNeighborsFromTrapezoid(belowSegmentNewFaces.at(j), {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

                for(auto k=i; k<=j; k++) {
                    if(finalNewFaces[1].at(k-1))
                        delete finalNewFaces[1].at(k-1);
                    finalNewFaces[1].at(k-1) = mergedFace;
                }
                T.push_back(mergedFace);
                mergedFace->ctype = "bottom-merged complex";

                // Jump to the face after Fj
                i = j;
            }
            // Otherwise
            else {
                finalNewFaces[1].at(i-1) = belowSegmentNewFaces.at(i);
                T.push_back(finalNewFaces[1].at(i-1));
                finalNewFaces[1].at(i-1)->ctype = "bottom-NON-merged complex";

            }
        }

        T.push_back(lastFace);
        lastFace->ctype = "right-last complex";


        // Update the DAG
        for(size_t i = 0; i < N_FACES; i++) {
            if(i==0) {
                D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, firstFace, finalNewFaces[0].at(i), finalNewFaces[1].at(i), nullptr);
            } else if (i == N_FACES-1) {
                D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, nullptr, finalNewFaces[0].at(i), finalNewFaces[1].at(i), lastFace);
            } else {
                D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, nullptr, finalNewFaces[0].at(i), finalNewFaces[1].at(i), nullptr);
            }
        }

        // Clean
        // TODO To improve
        for(auto f : intersectingFaces) {
            auto tmp = f;
            assert(tmp->getPointerToDAG()!=nullptr);
//            intersectingFaces.clear();
            T.erase(std::remove(T.begin(), T.end(), tmp), T.end());
            if(tmp)
                delete tmp;
//            T.erase(std::remove(T.begin(), T.end(), tmp), T.end());
        }
        intersectingFaces.clear();
        aboveSegmentNewFaces.clear();
        belowSegmentNewFaces.clear();
        finalNewFaces.clear();
    }
    /**/
}


// ----------------------- END PRIVATE SECTION -----------------------
