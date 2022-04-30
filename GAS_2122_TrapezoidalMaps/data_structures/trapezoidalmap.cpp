#include "trapezoidalmap.h"

// ----------------------- PUBLIC SECTION -----------------------


TrapezoidalMap::TrapezoidalMap() {}


void TrapezoidalMap::initialize(const cg3::BoundingBox2& B)
{

    // Adding the verteces of the BB
    auto topleft     = cg3::Point2d(B.min()+B.lengthY());
    auto topright    = cg3::Point2d(B.max());
    auto bottomleft  = cg3::Point2d(B.min());
    auto bottomright = cg3::Point2d(B.min()+B.lengthX());

    // Create a trapezoid from the bounding box
    auto top = OrderedSegment(topleft, topright);
    auto bottom = OrderedSegment(bottomleft, bottomright);
    Trapezoid boundingbox_trapezoid = Trapezoid(top, bottom, bottomleft, topright);

    // Initialize the trapezoidal map structure T and search structure D
    T = std::vector<Trapezoid*>();
    T.push_back(&boundingbox_trapezoid);
    D.initialize(&boundingbox_trapezoid);
}


void TrapezoidalMap::addSegment(OrderedSegment& segment) {
    // 1) Find the faces in the trapezoidal map T that intersect the segment
    auto facesIntersected = std::vector<Trapezoid*>();
    followSegment(segment, facesIntersected);
    split(segment, facesIntersected);
}
// ----------------------- END PUBLIC SECTION -----------------------




// ----------------------- PRIVATE SECTION -----------------------
void TrapezoidalMap::followSegment(OrderedSegment& s, std::vector<Trapezoid*> facesIntersectingSegment) {
    // 1. Let p and q be the left and right endpoint of s.
    auto p = s.getLeftmost();
    auto q = s.getRightmost();
    // 2. Search with p in the search structure D to find d0.
    Trapezoid* currentFace = D.query(p);

    facesIntersectingSegment.push_back(currentFace);

    //  while q lies to the right of rightp(dj)
    while(q > currentFace->getRightp()) {
        Position pos = OrientationUtility::getPointPositionRespectToLine(currentFace->getRightp(), s);
        // if rightp(dj) lies above the segment => go on the LowRight neighbor
        if(pos == above) {
            currentFace = currentFace->getLowerRightNeighbor();
        }
        // else if rightp(dj) lies below the segment => go on the TopRight neighbor
        else if (pos == below) {
            currentFace = currentFace->getUpperRightNeighbor();
        }
        // This case should be impossible ?
        else {
            assert(false);
        }

        //
        facesIntersectingSegment.push_back(currentFace);
    }
}


void TrapezoidalMap::split(OrderedSegment& s, std::vector<Trapezoid*> intersectingFaces) {
// it's better to pass the whole list to the function instead of a single trapezoid (because there are several scenarios to handle...)
    const auto N_FACES = intersectingFaces.size();
    /* EASIEST CASE */
    if(N_FACES== 1) {
        Trapezoid* oldFace = intersectingFaces[0];

        // left
        Trapezoid leftNewFace = Trapezoid(oldFace->getTop(), oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());
        // top
        Trapezoid topNewFace = Trapezoid(oldFace->getTop(), s, s.getLeftmost(), s.getRightmost());
        // right
        Trapezoid rightNewFace = Trapezoid(oldFace->getTop(), oldFace->getBottom(), s.getRightmost(), oldFace->getRightp());
        // bottom
        Trapezoid bottomNewFace = Trapezoid(s, oldFace->getBottom(), s.getLeftmost(), s.getRightmost());

        // setting adjacencies for:
        // left
        leftNewFace.replaceNeighborsFromTrapezoid(oldFace, {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
//        leftNewFace.setUpperLeftNeighbor(oldFace->getUpperLeftNeighbor());
//        leftNewFace.setLowerLeftNeighbor(oldFace->getLowerLeftNeighbor());
        leftNewFace.setUpperRightNeighbor(&topNewFace);
        leftNewFace.setLowerRightNeighbor(&bottomNewFace);

        // top
        topNewFace.setUpperLeftNeighbor(&leftNewFace);
//        topNewFace.setLowerLeftNeighbor(onlyFace->getLowerLeftNeighbor()); does the LL exist?
        topNewFace.setUpperRightNeighbor(&rightNewFace);
//        topNewFace.setLowerRightNeighbor(bottomNewFace); does the LR exist?

        // right
        rightNewFace.setUpperLeftNeighbor(&topNewFace);
        rightNewFace.setLowerLeftNeighbor(&bottomNewFace);
        rightNewFace.replaceNeighborsFromTrapezoid(oldFace, {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

//        rightNewFace.setUpperRightNeighbor(oldFace->getUpperRightNeighbor());
//        rightNewFace.setLowerRightNeighbor(oldFace->getLowerRightNeighbor());

        // bottom
        bottomNewFace.setLowerLeftNeighbor(&leftNewFace);
        bottomNewFace.setLowerRightNeighbor(&rightNewFace);
        //TODO      do UR and UL exist?

        // Update the faces (if not null) that had the old face as neighbor
//        if(oldFace->getUpperLeftNeighbor())
//            oldFace->getUpperLeftNeighbor()->replaceNeighbor(oldFace, &leftNewFace);
//        if(oldFace->getLowerLeftNeighbor())
//            oldFace->getLowerLeftNeighbor()->replaceNeighbor(oldFace, &leftNewFace);
//        if(oldFace->getUpperRightNeighbor())
//            oldFace->getUpperRightNeighbor()->replaceNeighbor(oldFace, &rightNewFace);
//        if(oldFace->getLowerRightNeighbor())
//            oldFace->getLowerRightNeighbor()->replaceNeighbor(oldFace, &rightNewFace);

        // Add the new faces on the trapezoidal map
         std::vector<Trapezoid*> tmpNewFaces = {&leftNewFace, &topNewFace, &rightNewFace, &bottomNewFace};
        assert(tmpNewFaces.size() == 4);
        /*T.push_back(&leftNewFace);
        T.push_back(&topNewFace);
        T.push_back(&rightNewFace);
        T.push_back(&bottomNewFace);*/
        T.insert(T.end(), tmpNewFaces.begin(), tmpNewFaces.end());

        // Upgrade the DAG
        D.replaceNodeWithSubtree(oldFace->getPointerToDAG(), s, tmpNewFaces);

        // Remove the old one from T
        auto d = T.size();
        T.erase(intersectingFaces.begin());
        assert(intersectingFaces.empty() == true);
        assert(T.size() == (d-1));
    }
    /* HARDEST CASE */
    else {
        std::vector<Trapezoid*> aboveSegmentNewFaces(N_FACES+2);
        std::vector<Trapezoid*> belowSegmentNewFaces(N_FACES+2);
//        std::vector<Trapezoid*> finalNewFaces(2*N_FACES+2);
        std::vector<Trapezoid*> finalNewFaces(2);

        /* SPLIT THE OLD FACES */
        for(auto i = 0; i<N_FACES; i++){
            Trapezoid* oldFace = intersectingFaces[i];
            // first face
            if(i==0){
                /* split it into three parts: leftNewFace, topNewFace, bottomNewFace*/
                // left
                Trapezoid leftNewFace = Trapezoid(oldFace->getTop(), oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());
                leftNewFace.replaceNeighborsFromTrapezoid(intersectingFaces[i], {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                // top
                Trapezoid topNewFace = Trapezoid(oldFace->getTop(), s, s.getLeftmost(), oldFace->getRightp());
                // bottom
                Trapezoid bottomNewFace = Trapezoid(s, oldFace->getBottom(), s.getLeftmost(), oldFace->getRightp());

                //
//                finalNewFaces.push_back(&leftNewFace);
                aboveSegmentNewFaces.push_back(&leftNewFace);
                belowSegmentNewFaces.push_back(&leftNewFace);

                aboveSegmentNewFaces.push_back(&topNewFace);
                belowSegmentNewFaces.push_back(&bottomNewFace);
            }
            // last face
            else if(i==intersectingFaces.size()-1) {
                /* split it into three parts: topNewFace, bottomNewFace, rightNewFace*/
                // right
                Trapezoid rightNewFace = Trapezoid(oldFace->getTop(), oldFace->getBottom(), s.getRightmost(), oldFace->getRightp());
                rightNewFace.replaceNeighborsFromTrapezoid(intersectingFaces[i], {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

                // top
                Trapezoid topNewFace = Trapezoid(oldFace->getTop(), s, oldFace->getLeftp(), s.getLeftmost());
                // bottom
                Trapezoid bottomNewFace = Trapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());

                //
                aboveSegmentNewFaces.push_back(&topNewFace);
                belowSegmentNewFaces.push_back(&bottomNewFace);

                aboveSegmentNewFaces.push_back(&rightNewFace);
                belowSegmentNewFaces.push_back(&rightNewFace);


            }
            // faces 1...n-1
            else {
                /* split it into two parts: topNewFace, bottomNewFace*/
                // top
                Trapezoid topNewFace = Trapezoid(oldFace->getTop(), s, oldFace->getLeftp(), oldFace->getRightp());
                // bottom
                Trapezoid bottomNewFace = Trapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), oldFace->getRightp());

                //
                aboveSegmentNewFaces.push_back(&topNewFace);
                belowSegmentNewFaces.push_back(&bottomNewFace);
            }
        }

        // Set the neighbors of the new faces
        for(auto i = 1; i < N_FACES+1; i++) {
            /* ABOVE */
            auto tmpTopFace = aboveSegmentNewFaces[i];
            // UpperRight
//            tmpTopFace->setUpperRightNeighbor(intersectingFaces[i]->getUpperRightNeighbor());
//            if(intersectingFaces[i]->getUpperRightNeighbor() != nullptr)
//                intersectingFaces[i]->getUpperRightNeighbor()->replaceNeighbor(intersectingFaces[i], tmpTopFace);
//            // LowerRight
            tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces[i+1]);
            // LowerLeft
            tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces[i-1]);
            // UpperLeft
//            tmpTopFace->setUpperLeftNeighbor(intersectingFaces[i]->getUpperLeftNeighbor());
//            if(intersectingFaces[i]->getUpperLeftNeighbor() != nullptr)
//                intersectingFaces[i]->getUpperLeftNeighbor()->replaceNeighbor(intersectingFaces[i], tmpTopFace);
            tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces[i], {Trapezoid::TOPLEFT, Trapezoid::TOPRIGHT});
            /* BELOW */
            auto tmpBottomFace = belowSegmentNewFaces[i];

            // UpperRight
            tmpBottomFace->setUpperRightNeighbor(aboveSegmentNewFaces[i+1]);
            // UpperLeft
            tmpBottomFace->setUpperLeftNeighbor(aboveSegmentNewFaces[i-1]);
            // LowerLeft
//            tmpBottomFace->setLowerLeftNeighbor(intersectingFaces[i]->getLowerLeftNeighbor());
//            if(intersectingFaces[i]->getLowerLeftNeighbor() != nullptr)
//                intersectingFaces[i]->getLowerLeftNeighbor()->replaceNeighbor(intersectingFaces[i], tmpBottomFace);
            // LowerRight
//            tmpBottomFace->setLowerRightNeighbor(intersectingFaces[i]->getLowerRightNeighbor());
//            if(intersectingFaces[i]->getLowerRightNeighbor() != nullptr)
//                intersectingFaces[i]->getLowerRightNeighbor()->replaceNeighbor(intersectingFaces[i], tmpBottomFace);
            tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces[i], {Trapezoid::BOTTOMLEFT, Trapezoid::BOTTOMRIGHT});
        }

        // Merging the eventually adjacent faces with same bottom and top
        finalNewFaces.push_back(aboveSegmentNewFaces.begin());

        /* ABOVE */
        for(auto i = 1; i < N_FACES+1; i++) {
            int j = i;
            // TODO == operator of OrderedSegment should be ok since it was inherited by cg3::Segment(?)
            while(j<N_FACES+1
                  && aboveSegmentNewFaces[j]->getTop()    == aboveSegmentNewFaces[i]->getTop()
                  && aboveSegmentNewFaces[j]->getBottom() == aboveSegmentNewFaces[i]->getBottom())
                j++;

            // If I found faces to merge
            if(j != i) {
                // Create the new face
                Trapezoid mergedFace = Trapezoid(aboveSegmentNewFaces[i]->getTop(), aboveSegmentNewFaces[i]->getBottom(), aboveSegmentNewFaces[i]->getLeftp(), aboveSegmentNewFaces[j]->getRightp());
                // Set the neighbors
                mergedFace.replaceNeighborsFromTrapezoid(aboveSegmentNewFaces[i], {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                mergedFace.replaceNeighborsFromTrapezoid(aboveSegmentNewFaces[j], {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

                finalNewFaces.push_back(&mergedFace);

                // Jump to the face after Fj
                i = j;
            }
            // Otherwise
            else {
                finalNewFaces.push_back(aboveSegmentNewFaces[i]);
            }
        }

        /* BELOW */
        for(auto i = 1; i < N_FACES+1; i++) {
            int j = i;
            // TODO == operator of OrderedSegment should be ok since it was inherited by cg3::Segment(?)
            while(j<N_FACES+1
                  && belowSegmentNewFaces[j]->getTop()    == belowSegmentNewFaces[i]->getTop()
                  && belowSegmentNewFaces[j]->getBottom() == belowSegmentNewFaces[i]->getBottom())
                j++;

            // If I found faces to merge
            if(j != i) {
                // Create the new face
                Trapezoid mergedFace = Trapezoid(belowSegmentNewFaces[i]->getTop(), belowSegmentNewFaces[i]->getBottom(), belowSegmentNewFaces[i]->getLeftp(), belowSegmentNewFaces[j]->getRightp());
                // Set the neighbors
                mergedFace.replaceNeighborsFromTrapezoid(belowSegmentNewFaces[i], {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                mergedFace.replaceNeighborsFromTrapezoid(belowSegmentNewFaces[j], {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

                finalNewFaces.push_back(&mergedFace);

                // Jump to the face after Fj
                i = j;
            }
            // Otherwise
            else {
                finalNewFaces.push_back(belowSegmentNewFaces[i]);
            }
        }
        finalNewFaces.push_back(aboveSegmentNewFaces.back());
    }
    /**/
}


// ----------------------- END PRIVATE SECTION -----------------------
