#include "trapezoidalmap.h"

#include "cg3/geometry/utils2.h"
// ----------------------- PUBLIC SECTION -----------------------
TrapezoidalMap::~TrapezoidalMap() {
    // deleting the dag
    D.~DAG();

    // de-allocate the faces and remove them from the trapezoidal map
    for (auto iterable_face=T.begin(); iterable_face != T.end(); iterable_face++)
        if(*iterable_face)
            delete *iterable_face;
    T.clear();
    T.shrink_to_fit();

    // de-allocate and remove the segments (and therefore the endpoints)
    for (auto iterable_segment=segments.begin(); iterable_segment != segments.end(); iterable_segment++)
        if(*iterable_segment)
            delete *iterable_segment;
    segments.clear();
    segments.shrink_to_fit();
}

TrapezoidalMap::TrapezoidalMap() {}

void TrapezoidalMap::initialize(const cg3::BoundingBox2& B)
{
    // Saving the boundary
    setBoundingBox(B);

    // Create a trapezoid from the bounding box
    auto topleft     = cg3::Point2d(B.min().x(), B.max().y());
    auto topright    = cg3::Point2d(B.max());
    auto bottomleft  = cg3::Point2d(B.min());
    auto bottomright = cg3::Point2d(B.max().x(), B.min().y());
    OrderedSegment* top = new OrderedSegment(topleft, topright);
    OrderedSegment* bottom = new OrderedSegment(bottomleft, bottomright);
    DrawableTrapezoid*  boundingbox_trapezoid = new DrawableTrapezoid(*top, *bottom, bottomleft, topright);
    // Push the trapezoid inside the map
    this->addTrapezoidToMap(boundingbox_trapezoid);

    // Push the 2 segments into the segment list
    segments.push_back(top);
    segments.push_back(bottom);

    // Initialize the DAG with the trapezoid
    D.initialize(boundingbox_trapezoid);
}


void TrapezoidalMap::addSegment(const cg3::Segment2d& segment) {
    // The segment we receive is a stack object, so its memory will be freed at the end of the insertion: I need to save it dinamically
    // N.B. the new segment is ordered.
    OrderedSegment* orderedSegment = new OrderedSegment(segment);
    // Save the segment into the segment list
    segments.push_back(orderedSegment);

    // Find the faces in the trapezoidal map T that intersect the segment, sorted from left to right
    auto facesIntersected = std::vector<DrawableTrapezoid*>();
    followSegment(*orderedSegment , facesIntersected);
    // Split those faces and update the map/dag with the new faces
    split(*orderedSegment , facesIntersected);
}

DrawableTrapezoid* TrapezoidalMap::pointLocation(const cg3::Point2d& pointToQuery) const {
    return D.queryFaceContaininingPoint(pointToQuery);
}

void TrapezoidalMap::clear() {
    // Cleaning the data dynamically instantiated
    this->~TrapezoidalMap();
}

void TrapezoidalMap::reset() {
    // First of all, clear the data structures
    this->clear();

    // Secondly, initiale them again using the boundary box previously saved.
    this->initialize(this->getBoundingBox());
}
// ----------------------- END PUBLIC SECTION -----------------------



// ------------------------- PROTECTED SECTION -------------------------
const cg3::BoundingBox2 &TrapezoidalMap::getBoundingBox() const
{
    return B;
}
// ----------------------- END PROTECTED SECTION -----------------------




// ----------------------- PRIVATE SECTION -----------------------
void TrapezoidalMap::setBoundingBox(const cg3::BoundingBox2 &newB)
{
    B = newB;
}

void TrapezoidalMap::followSegment(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& facesIntersectingSegment) const {
    // 1. Let p and q be the left and right endpoint of the segment.
    auto p = s.getLeftmost();
    auto q = s.getRightmost();

    // 2. Search with p in the search structure D to find d0.
    DrawableTrapezoid*  currentFace = D.queryLeftmostFaceIntersectingSegment(s);
    facesIntersectingSegment.push_back(currentFace);
    currentFace->setIsBeingSplitted(true);

    //  while q lies to the right of rightp(dj)
    while(currentFace != nullptr && q.x() > currentFace->getRightp().x()) {

        // if rightp(dj) lies above the segment => go on the LowerRight neighbor
        if(cg3::isPointAtLeft(s.getLeftmost(), s.getRightmost(), currentFace->getRightp())) {
            currentFace = (DrawableTrapezoid*)currentFace->getLowerRightNeighbor();
        }
        // else if rightp(dj) lies below the segment => go on the UpperRight neighbor
        else if(cg3::isPointAtRight(s.getLeftmost(), s.getRightmost(), currentFace->getRightp())) {
            currentFace = (DrawableTrapezoid*)currentFace->getUpperRightNeighbor();
        }
        /* else rightp(dj) is on the segment.
         * This can happen only if q.x = rightp.x, but this condition is impossible since we already checked that q.x > rightp.x,
         * so this case should be impossible */
        else {
            assert(false);
        }


        if(currentFace != nullptr) {
            facesIntersectingSegment.push_back(currentFace);
            currentFace->setIsBeingSplitted(true);
        }
    }
}


void TrapezoidalMap::split(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces) {
    /* Split the faces, insert the new ones into the trapezoidal map and update the DAG. */
    if(intersectingFaces.size()== 1) {
        splitSingularTrapezoid(s, intersectingFaces.front());
    } else {
        splitMultipleTrapezoid(s, intersectingFaces);
    }


    /* At the end of the splitting, delete the old faces from the trapezoidal map.*/
    for(auto f : intersectingFaces)
        deleteTrapezoidFromMap(f);
    intersectingFaces.clear();
}

void TrapezoidalMap::splitSingularTrapezoid(const OrderedSegment& s, DrawableTrapezoid* faceToSplit) {
    DrawableTrapezoid *leftNewFace, *topNewFace, *bottomNewFace, *rightNewFace;

    // if the leftmost endpoint of the segment is equal to the left point of the trapezoid to split, do NOT create the left face
    bool leftFaceExists  = s.getLeftmost()  != faceToSplit->getLeftp();
    // if the rightmost endpoint of the segment is equal to the right point of the trapezoid to split, do NOT create the right face
    bool rightFaceExists = s.getRightmost() != faceToSplit->getRightp();

    // Creating the faces: from 2 to 4
    leftNewFace = leftFaceExists
            ? new DrawableTrapezoid(faceToSplit->getTop(), faceToSplit->getBottom(), faceToSplit->getLeftp(), s.getLeftmost())
            : nullptr;
    topNewFace = new DrawableTrapezoid(faceToSplit->getTop(), s, s.getLeftmost(), s.getRightmost());
    bottomNewFace = new DrawableTrapezoid(s, faceToSplit->getBottom(), s.getLeftmost(), s.getRightmost());
    rightNewFace = rightFaceExists
            ? new DrawableTrapezoid(faceToSplit->getTop(), faceToSplit->getBottom(), s.getRightmost(), faceToSplit->getRightp())
            : nullptr;

    /* SETTING THE ADJACENCIES FOR THE NEW FACES */
    // left
    if(leftFaceExists) {
        leftNewFace->replaceNeighborsFromTrapezoid(faceToSplit, {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
        leftNewFace->setUpperRightNeighbor(topNewFace);
        leftNewFace->setLowerRightNeighbor(bottomNewFace);
    }

    // top
    if(leftFaceExists)
        topNewFace->setUpperLeftNeighbor(leftNewFace);
    else
        topNewFace->replaceNeighborsFromTrapezoid(faceToSplit, {Trapezoid::TOPLEFT});

    if(rightFaceExists)
        topNewFace->setUpperRightNeighbor(rightNewFace);
    else
        topNewFace->replaceNeighborsFromTrapezoid(faceToSplit, {Trapezoid::TOPRIGHT});

    // right
    if(rightFaceExists) {
        rightNewFace->setUpperLeftNeighbor(topNewFace);
        rightNewFace->setLowerLeftNeighbor(bottomNewFace);
        rightNewFace->replaceNeighborsFromTrapezoid(faceToSplit, {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});
    }

    // bottom
    if(leftFaceExists)
        bottomNewFace->setLowerLeftNeighbor(leftNewFace);
    else
        bottomNewFace->replaceNeighborsFromTrapezoid(faceToSplit, {Trapezoid::BOTTOMLEFT});

    if(rightFaceExists)
        bottomNewFace->setLowerRightNeighbor(rightNewFace);
    else
        bottomNewFace->replaceNeighborsFromTrapezoid(faceToSplit, {Trapezoid::BOTTOMRIGHT});


    // Add the new faces into the trapezoidal map
    if(leftFaceExists)
        this->addTrapezoidToMap(leftNewFace);
    this->addTrapezoidToMap(topNewFace);
    this->addTrapezoidToMap(bottomNewFace);
    if(rightFaceExists)
        this->addTrapezoidToMap(rightNewFace);

    // Upgrade the DAG leaf pointing to the old trapezoid
    D.replaceNodeWithSubtree(faceToSplit->getPointerToDAG(), s, leftNewFace, topNewFace, bottomNewFace, rightNewFace);
}

void TrapezoidalMap::splitMultipleTrapezoid(const OrderedSegment& s, std::vector<DrawableTrapezoid*>& intersectingFaces) {
    DrawableTrapezoid *firstFace = nullptr, *lastFace = nullptr, *topNewFace, *bottomNewFace;
    const size_t N_FACES = intersectingFaces.size();
    // the faces above the segment are managed differently from the faces below the segment, so I save them in two sepated lists
    std::vector<DrawableTrapezoid*> aboveSegmentNewFaces(N_FACES);
    std::vector<DrawableTrapezoid*> belowSegmentNewFaces(N_FACES);

    // if the leftmost endpoint of the segment is equal to the left point of the first trapezoid to split, do NOT create the left face
    bool firstFaceExists = s.getLeftmost() != intersectingFaces.front()->getLeftp();
    // if the rightmost endpoint of the segment is equal to the right point of the last trapezoid to split, do NOT create the right face
    bool lastFaceExists =  s.getRightmost() != intersectingFaces.back()->getRightp();

    /* SPLIT THE OLD FACES */
    for(size_t i = 0; i<intersectingFaces.size(); i++){
        DrawableTrapezoid*  oldFace = intersectingFaces.at(i);
        // first face of the list
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
        // last face of the list
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

        // faces 1...n-1 of the list
        else {
            /* split it into two parts: topNewFace, bottomNewFace*/
            topNewFace = new DrawableTrapezoid(oldFace->getTop(), s, oldFace->getLeftp(), oldFace->getRightp());
            bottomNewFace = new DrawableTrapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), oldFace->getRightp());
        }

        // Push the top face in the "list containing the faces ABOVE the segment"
        aboveSegmentNewFaces.at(i) = topNewFace;
        // Push the bottom face in the "list containing the faces BELOW the segment"
        belowSegmentNewFaces.at(i) = bottomNewFace;
    }

    /* SET THE NEIGHBORS OF THE NEW FACES */
    /* Faces above the segment */
    for(size_t i = 0; i < aboveSegmentNewFaces.size(); i++) {
        DrawableTrapezoid*  tmpTopFace = aboveSegmentNewFaces.at(i);
        // first top face
        if(i==0) {
            // if the old face has an upper right neighbor that's not a split face, link the top face to it
            if(intersectingFaces.at(i)->getUpperRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperRightNeighbor()->getIsBeingSplitted()) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT});
            }

            // the lower right neighbor is the next top face
            tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces.at(i+1));

            // if the first face exists, link the top face to it, otherwise link it to the top left neighbor of the old face
            if(firstFaceExists)
                tmpTopFace->setUpperLeftNeighbor(firstFace);
            else {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT});
            }

        }
        // last top face
        else if (i==aboveSegmentNewFaces.size()-1) {
            // if the last face exists, link the top face to it, otherwise link it to the top left neighbor of the old face
            if (lastFaceExists) {
                tmpTopFace->setUpperRightNeighbor(lastFace);
            }
            else {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT});
            }

            // the lower left neighbor is the previous top face
            tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces.at(i-1));

            // if the old face has an upper left neighbor that's not a split face, link the top face to it
            if(intersectingFaces.at(i)->getUpperLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperLeftNeighbor()->getIsBeingSplitted()) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT});
            }
        }
        // 1...k-1 top faces
        else {
            // if the old face has an upper right neighbor that's not a split face, link the top face to it
            if(intersectingFaces.at(i)->getUpperRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperRightNeighbor()->getIsBeingSplitted()) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT});
            }

            // the lower right neighbor is the next top face
            tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces.at(i+1));
            // the lower left neighbor is the previous top face
            tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces.at(i-1));


            // if the old face has an upper left neighbor that's not a split face, link the top face to it
            if(intersectingFaces.at(i)->getUpperLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getUpperLeftNeighbor()->getIsBeingSplitted()) {
                tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT});
            }
        }
    }

    /* Faces below the segment */
    for(size_t i = 0; i < belowSegmentNewFaces.size(); i++) {
        DrawableTrapezoid*  tmpBottomFace = belowSegmentNewFaces.at(i);
        // first bottom face
        if(i==0) {
            // the upper right neighbor is the next bottom face
            tmpBottomFace->setUpperRightNeighbor(belowSegmentNewFaces.at(i+1));

            // if the old face has a lower right neighbor that's not a split face, link the bottom face to it
            if(intersectingFaces.at(i)->getLowerRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerRightNeighbor()->getIsBeingSplitted()) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMRIGHT});
            }

            // if the first face exists, link the bottom face to it, otherwise link it to the bottom left neighbor of the old face
            if(firstFaceExists)
                tmpBottomFace->setLowerLeftNeighbor(firstFace);
            else {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMLEFT});
            }

        }
        // last bottom face
        else if (i==belowSegmentNewFaces.size()-1) {
            // if the last face exists, link the bottom face to it, otherwise link it to the bottom right neighbor of the old face
            if(lastFaceExists)
                tmpBottomFace->setLowerRightNeighbor(lastFace);
            else {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMRIGHT});
            }

            // if the old face has a lower left neighbor that's not a split face, link the bottom face to it
            if(intersectingFaces.at(i)->getLowerLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerLeftNeighbor()->getIsBeingSplitted()) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMLEFT});
            }

            // the upper left neighbor is the previous bottom face
            tmpBottomFace->setUpperLeftNeighbor(belowSegmentNewFaces.at(i-1));
        }
        // 1...k-1 bottom faces
        else {
            // the upper right neighbor is the next bottom face
            tmpBottomFace->setUpperRightNeighbor(belowSegmentNewFaces.at(i+1));

            // if the old face has a lower right neighbor that's not a split face, link the bottom face to it
            if(intersectingFaces.at(i)->getLowerRightNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerRightNeighbor()->getIsBeingSplitted()) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMRIGHT});
            }

            // if the old face has a lower left neighbor that's not a split face, link the bottom face to it
            if(intersectingFaces.at(i)->getLowerLeftNeighbor()!=nullptr && !intersectingFaces.at(i)->getLowerLeftNeighbor()->getIsBeingSplitted()) {
                tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::BOTTOMLEFT});
            }

            // the upper left neighbor is the previous bottom face
            tmpBottomFace->setUpperLeftNeighbor(belowSegmentNewFaces.at(i-1));
        }
    }

    /* MERGING (IF NEEDED) AND PUSHING THE FINAL FACES INTO THE TRAPEZOIDAL MAP */
    /* Faces above the segment */
    if(firstFaceExists)
        this->addTrapezoidToMap(firstFace);
    stepMerging(0, N_FACES, aboveSegmentNewFaces);

    /* Faces below the segment */
    stepMerging(0, N_FACES, belowSegmentNewFaces);
    if(lastFaceExists)
        this->addTrapezoidToMap(lastFace);

    // UPDATING THE DAG
    for(size_t i = 0; i < N_FACES; i++) {
        auto tmpLeft = (i==0 && firstFaceExists) ? firstFace : nullptr;
        auto tmpRight = (i==N_FACES-1 && lastFaceExists) ? lastFace : nullptr;
        D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, tmpLeft, aboveSegmentNewFaces.at(i), belowSegmentNewFaces.at(i), tmpRight);
    }

    // Empty the two lists used in this function.
    aboveSegmentNewFaces.clear();
    belowSegmentNewFaces.clear();
}

void TrapezoidalMap::stepMerging(size_t start, size_t end, std::vector<DrawableTrapezoid*>& list) {
    for(size_t i = start; i < end; i++) {
        /* search if we can merge the current trapezoid with the next, and then with the next and so on...
                i = index of the current face
                j = index of the rightmost face mergeable */
        size_t j = i;
        while(j<end-1 && Trapezoid::canMerge(*list.at(j), *list.at(j+1)))
            j++;

        // If I found faces to merge with
        if(j != i) {

            /* Create the new face, it has:
             *      the left point of the leftmost trapezoid   (i)
             *      the right point of the rightmost trapezoid (j)
             *      (P.S. the top and bottom segment are the same in all the mergeable faces)
            */
            DrawableTrapezoid*  mergedFace = new DrawableTrapezoid(list.at(i)->getTop(), list.at(i)->getBottom(), list.at(i)->getLeftp(), list.at(j)->getRightp());
            /* Set its neighbors. They will be
             *          the left neighbors from the leftmost trapezoid
             *          the right neighbors from the rightmost trapezoid
            */
            mergedFace->replaceNeighborsFromTrapezoid(list.at(i), {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
            mergedFace->replaceNeighborsFromTrapezoid(list.at(j), {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

            // Replace the old merged trapezoids with the new one.
            // Deallocate the old merged trapezoids (from i to j).
            for(auto k=i; k<=j; k++) {
                if(list.at(k)) {
                    delete list.at(k);
                    list.at(k) = nullptr;
                }
                list.at(k) = mergedFace;
            }

            // We want to star from the face immediately after Fj
            i = j;

            // Insert the merged face into the trapezoidal map
            this->addTrapezoidToMap(mergedFace);

        }
        // Otherwise push the non-mergeable face into the trapezoidal map
        else {
            this->addTrapezoidToMap(list.at(i));
        }
    }
}

void TrapezoidalMap::addTrapezoidToMap(DrawableTrapezoid* trapezoidToAdd) {
    assert(trapezoidToAdd != nullptr);

    // if the graphics has not been calculated yet, do it now
    if(!trapezoidToAdd->isGraphicsCalculated())
        trapezoidToAdd->calculateGraphics();

    assert(trapezoidToAdd->isGraphicsCalculated());

    // Push the trapezoid into the list
    T.push_back(trapezoidToAdd);
}

void TrapezoidalMap::deleteTrapezoidFromMap(DrawableTrapezoid* trapezoidToDelete) {
    assert(trapezoidToDelete != nullptr);

    assert(trapezoidToDelete->getPointerToDAG()!=nullptr);
    T.erase(std::remove(T.begin(), T.end(), trapezoidToDelete), T.end());
    if(trapezoidToDelete)
        delete trapezoidToDelete;

}
// ----------------------- END PRIVATE SECTION -----------------------
