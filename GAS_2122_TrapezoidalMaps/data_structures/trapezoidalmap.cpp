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

    // Create a trapezoid from the bounding box
    //Trapezoid* boundingbox_trapezoid = Trapezoid::generateTrapezoid(B);
    auto topleft     = cg3::Point2d(B.min().x(), B.max().y());
    auto topright    = cg3::Point2d(B.max());
    auto bottomleft  = cg3::Point2d(B.min());
    auto bottomright = cg3::Point2d(B.max().x(), B.min().y());
    OrderedSegment* top = new OrderedSegment(topleft, topright);
    OrderedSegment* bottom = new OrderedSegment(bottomleft, bottomright);
    DrawableTrapezoid*  boundingbox_trapezoid = new DrawableTrapezoid(*top, *bottom, bottomleft, topright);
    segments.push_back(top);
    segments.push_back(bottom);

    // Initialize the trapezoidal map structure T and search structure D
    //T = std::vector<Trapezoid*>();
    ///T.push_back(boundingbox_trapezoid);
    this->addTrapezoidToMap(boundingbox_trapezoid);
    D.initialize(boundingbox_trapezoid);
}


void TrapezoidalMap::addSegment(const cg3::Segment2d& segment) {
    // Find the faces in the trapezoidal map T that intersect the segment
    // sorted from left to right
    ///TODO D.tmp();
    OrderedSegment* orderedSegment = new OrderedSegment(segment);
    segments.push_back(orderedSegment);

    auto facesIntersected = std::vector<DrawableTrapezoid*>();
    followSegment(*orderedSegment , facesIntersected);
    // Split those faces and update the map/dag with the new faces
    split(*orderedSegment , facesIntersected);
}

DrawableTrapezoid* TrapezoidalMap::pointLocation(const cg3::Point2d& pointToQuery) {
    return (DrawableTrapezoid*) D.queryFaceContaininingPoint(pointToQuery);
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
void TrapezoidalMap::followSegment(OrderedSegment& s, std::vector<DrawableTrapezoid*>& facesIntersectingSegment) {
    // 1. Let p and q be the left and right endpoint of s.
    auto p = s.getLeftmost();
    auto q = s.getRightmost();
    // 2. Search with p in the search structure D to find d0.
    DrawableTrapezoid*  currentFace = D.queryLeftmostFaceIntersectingSegment(s);

    facesIntersectingSegment.push_back(currentFace);

    currentFace->isBeingSplitted = true;
    //  while q lies to the right of rightp(dj)
    while(currentFace != nullptr && q.x() > currentFace->getRightp().x()) {
        ///Position pos = OrientationUtility::getPointPositionRespectToLine(tmp->getRightp(), s);
        // if rightp(dj) lies above the segment => go on the LowRight neighbor
        ///if(pos == above) {
        if(cg3::isPointAtLeft(s.getLeftmost(), s.getRightmost(), currentFace->getRightp())) {
            currentFace = (DrawableTrapezoid*)currentFace->getLowerRightNeighbor();
        }
        // else if rightp(dj) lies below the segment => go on the TopRight neighbor
        ///else if (pos == below) {
        else if(cg3::isPointAtRight(s.getLeftmost(), s.getRightmost(), currentFace->getRightp())) {
            currentFace = (DrawableTrapezoid*)currentFace->getUpperRightNeighbor();
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

//
void TrapezoidalMap::split(OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces) {
    // it's better to pass the whole list to the function instead of a single trapezoid (because there are several scenarios to handle...)

    if(intersectingFaces.size()== 1) {
        splitSingularTrapezoid(s, intersectingFaces.front());
    } else {
        splitMultipleTrapezoid(s, intersectingFaces);
    }
    /* */
    for(auto f : intersectingFaces) {
        auto tmp = f;
        assert(tmp->getPointerToDAG()!=nullptr);
        T.erase(std::remove(T.begin(), T.end(), tmp), T.end());
        if(tmp)
            delete tmp;
    }
    intersectingFaces.clear();
}
void TrapezoidalMap::splitSingularTrapezoid(OrderedSegment& s, DrawableTrapezoid* faceToSplit) {
    DrawableTrapezoid* oldFace = faceToSplit;

    // Creating the face
    DrawableTrapezoid* leftNewFace = new DrawableTrapezoid(oldFace->getTop(), oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());
    DrawableTrapezoid* topNewFace = new DrawableTrapezoid(oldFace->getTop(), s, s.getLeftmost(), s.getRightmost());
    DrawableTrapezoid*  bottomNewFace = new DrawableTrapezoid(s, oldFace->getBottom(), s.getLeftmost(), s.getRightmost());
    DrawableTrapezoid* rightNewFace = new DrawableTrapezoid(oldFace->getTop(), oldFace->getBottom(), s.getRightmost(), oldFace->getRightp());

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
    this->addTrapezoidToMap(leftNewFace);
    this->addTrapezoidToMap(topNewFace);
    this->addTrapezoidToMap(bottomNewFace);
    this->addTrapezoidToMap(rightNewFace);

    // Upgrade the DAG
    D.replaceNodeWithSubtree(oldFace->getPointerToDAG(), s, leftNewFace, topNewFace, bottomNewFace, rightNewFace);
}
void TrapezoidalMap::splitMultipleTrapezoid(OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces) {
    DrawableTrapezoid*  firstFace, *lastFace, *topNewFace, *bottomNewFace;
    const size_t N_FACES = intersectingFaces.size();
    std::vector<DrawableTrapezoid*> aboveSegmentNewFaces(N_FACES);
    std::vector<DrawableTrapezoid*> belowSegmentNewFaces(N_FACES);


    bool firstFaceExists = s.getLeftmost() != intersectingFaces.front()->getLeftp();
    bool lastFaceExists =  s.getRightmost() != intersectingFaces.back()->getRightp();

    /* SPLIT THE OLD FACES */
    for(size_t i = 0; i<intersectingFaces.size(); i++){
        DrawableTrapezoid*  oldFace = intersectingFaces.at(i);
        // first face
        if(intersectingFaces.at(i)==intersectingFaces.front()){
            /* General case:
             *  split it into 3 parts: leftNewFace, topNewFace, bottomNewFace
             * If the first face doesn't exist:
             *  split it into 2 parts: topNewFace, bottomNewFace
            */
            topNewFace = new DrawableTrapezoid(oldFace->getTop(), s, s.getLeftmost(), oldFace->getRightp());
            bottomNewFace = new DrawableTrapezoid(s, oldFace->getBottom(), s.getLeftmost(), oldFace->getRightp());
            if(firstFaceExists) {
                // Create the first face and set its 4 neighbors
                firstFace = new DrawableTrapezoid(oldFace->getTop(), oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());
                firstFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                firstFace->setUpperRightNeighbor(topNewFace);
                firstFace->setLowerRightNeighbor(bottomNewFace);
            }
        }
        // last face
        else if(intersectingFaces.at(i)==intersectingFaces.back()) {
            /* General case:
             *  split it into 3 parts: lastFace, topNewFace, bottomNewFace
             * If the last face doesn't exist:
             *  split it into 2 parts: topNewFace, bottomNewFace
            */
            topNewFace = new DrawableTrapezoid(oldFace->getTop(), s, oldFace->getLeftp(), s.getRightmost());
            bottomNewFace = new DrawableTrapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), s.getRightmost());
            if (lastFaceExists) {
                // Create the last face and set its 4 neighbors
                lastFace = new DrawableTrapezoid(oldFace->getTop(), oldFace->getBottom(), s.getRightmost(), oldFace->getRightp());

                lastFace->setUpperLeftNeighbor(topNewFace);
                lastFace->setLowerLeftNeighbor(bottomNewFace);
                lastFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});
            }
        }
        // faces 1...n-1
        else {
            /* split it into two parts: topNewFace, bottomNewFace*/
            topNewFace = new DrawableTrapezoid(oldFace->getTop(), s, oldFace->getLeftp(), oldFace->getRightp());
            bottomNewFace = new DrawableTrapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), oldFace->getRightp());
        }

        // Push top in the "list containing the faces ABOVE the segment"
        aboveSegmentNewFaces.at(i) = topNewFace;
        // Push top in the "list containing the faces BELOW the segment"
        belowSegmentNewFaces.at(i) = bottomNewFace;
    }

    /* SET THE NEIGHBORS OF THE NEW FACES */
    /* Faces above the segment */
    for(size_t i = 0; i < aboveSegmentNewFaces.size(); i++) {
        DrawableTrapezoid*  tmpTopFace = aboveSegmentNewFaces.at(i);

        if(i==0) {
            if(intersectingFaces.at(i)->getUpperRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperRightNeighbor()->isBeingSplitted) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT});
            }
            tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces.at(i+1));
            tmpTopFace->setLowerLeftNeighbor(nullptr); // can be deleted
            if(firstFaceExists)
                tmpTopFace->setUpperLeftNeighbor(firstFace); // == left face
            else {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT});
            }

        } else if (i==aboveSegmentNewFaces.size()-1) {
            if (lastFaceExists) {
                tmpTopFace->setUpperRightNeighbor(lastFace);
            }
            else {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT});
            }
            tmpTopFace->setLowerRightNeighbor(nullptr); // can be deleted
            tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces.at(i-1));
            if(intersectingFaces.at(i)->getUpperLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperLeftNeighbor()->isBeingSplitted) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT});
            }
        } else {
            if(intersectingFaces.at(i)->getUpperRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperRightNeighbor()->isBeingSplitted) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT});
            }
            tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces.at(i+1));
            tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces.at(i-1));
            if(intersectingFaces.at(i)->getUpperLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperLeftNeighbor()->isBeingSplitted) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT});
            }
        }
    }

    /* Faces below the segment */
    for(size_t i = 0; i < belowSegmentNewFaces.size(); i++) {
        DrawableTrapezoid*  tmpBottomFace = belowSegmentNewFaces.at(i);
        if(i==0) {
            tmpBottomFace->setUpperRightNeighbor(belowSegmentNewFaces.at(i+1));
            if(intersectingFaces.at(i)->getLowerRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerRightNeighbor()->isBeingSplitted) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMRIGHT});
            }
            if(firstFaceExists)
                tmpBottomFace->setLowerLeftNeighbor(firstFace); // == left face
            else {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMLEFT});
            }
            tmpBottomFace->setUpperLeftNeighbor(nullptr); // can be deleted

        } else if (i==belowSegmentNewFaces.size()-1) {
            tmpBottomFace->setUpperRightNeighbor(nullptr);
            if(lastFaceExists)
                tmpBottomFace->setLowerRightNeighbor(lastFace); // == last face
            else {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMRIGHT});
            }
            if(intersectingFaces.at(i)->getLowerLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerLeftNeighbor()->isBeingSplitted) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMLEFT});
            }
            tmpBottomFace->setUpperLeftNeighbor(belowSegmentNewFaces.at(i-1));
        } else {
            tmpBottomFace->setUpperRightNeighbor(belowSegmentNewFaces.at(i+1));
            if(intersectingFaces.at(i)->getLowerRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerRightNeighbor()->isBeingSplitted) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMRIGHT});
            }
            if(intersectingFaces.at(i)->getLowerLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerLeftNeighbor()->isBeingSplitted) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMLEFT});
            }
            tmpBottomFace->setUpperLeftNeighbor(belowSegmentNewFaces.at(i-1));
        }
    }

    /* MERGING (IF NEEDED) AND PUSHING THE FINAL FACE*/

    /* Faces above the segment */
    if(firstFaceExists)
        this->addTrapezoidToMap(firstFace);
    stepMerging(0, N_FACES, aboveSegmentNewFaces);

    /* Faces below the segment */
    stepMerging(0, N_FACES, belowSegmentNewFaces);
    if(lastFaceExists)
        this->addTrapezoidToMap(lastFace);


    // Update the DAG
    for(size_t i = 0; i < N_FACES; i++) {
        auto tmpLeft = (i==0 && firstFaceExists) ? firstFace : nullptr;
        auto tmpRight = (i==N_FACES-1 && lastFaceExists) ? lastFace : nullptr;
        D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, tmpLeft, aboveSegmentNewFaces.at(i), belowSegmentNewFaces.at(i), tmpRight);
    }

    // Clean
    aboveSegmentNewFaces.clear();
    belowSegmentNewFaces.clear();
}

void TrapezoidalMap::stepMerging(size_t start, size_t end, std::vector<DrawableTrapezoid*>& list) {
    for(size_t i = start; i < end; i++) {
        size_t j = i;
        while(j<end-1 && Trapezoid::canMerge(*list.at(j), *list.at(j+1)))
            j++;

        // If I found faces to merge
        if(j != i) {
            // Create the new face: it has the left point
            DrawableTrapezoid*  mergedFace = new DrawableTrapezoid(list.at(i)->getTop(), list.at(i)->getBottom(), list.at(i)->getLeftp(), list.at(j)->getRightp());
            // Set the neighbors
            mergedFace->replaceNeighborsFromTrapezoid(list.at(i), {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
            mergedFace->replaceNeighborsFromTrapezoid(list.at(j), {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

            for(auto k=i; k<=j; k++) {
                if(list.at(k)) {
                    delete list.at(k);
                    list.at(k) = nullptr;
                }
                list.at(k) = mergedFace;
            }
            // Jump to the face after Fj
            i = j;
            this->addTrapezoidToMap(mergedFace);

        }
        // Otherwise
        else {
            this->addTrapezoidToMap(list.at(i));
        }
    }
}
//
void TrapezoidalMap::addTrapezoidToMap(DrawableTrapezoid* trapezoidToAdd) {
    assert(trapezoidToAdd != nullptr);

    if(!trapezoidToAdd->isGraphicsCalculated())
        trapezoidToAdd->calculateGraphics();

    assert(trapezoidToAdd->isGraphicsCalculated());
    T.push_back(trapezoidToAdd);
}
// ----------------------- END PRIVATE SECTION -----------------------
