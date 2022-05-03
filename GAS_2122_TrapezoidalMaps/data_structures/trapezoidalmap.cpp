#include "trapezoidalmap.h"

#include "cg3/geometry/utils2.h"
// ----------------------- PUBLIC SECTION -----------------------


TrapezoidalMap::TrapezoidalMap() {}


void TrapezoidalMap::initialize(const cg3::BoundingBox2& B)
{

    // Saving the y-coordinates in the Trapezoid class
    auto yMin = B.min().y();
    auto yMax = B.max().y();
    Trapezoid::setYMax(yMax);
    Trapezoid::setYMin(yMin);

    // Create a trapezoid from the bounding box
    auto topleft     = cg3::Point2d(B.min().x(), yMax);
    auto topright    = cg3::Point2d(B.max());
    auto bottomleft  = cg3::Point2d(B.min());
    auto bottomright = cg3::Point2d(B.max().x(), yMin);
    auto top = OrderedSegment(topleft, topright);
    auto bottom = OrderedSegment(bottomleft, bottomright);
    Trapezoid* boundingbox_trapezoid = new Trapezoid(top, bottom, bottomleft, topright);

    // Initialize the trapezoidal map structure T and search structure D
    T = std::vector<Trapezoid*>();
    T.push_back(boundingbox_trapezoid);
    D.initialize(boundingbox_trapezoid);
}


void TrapezoidalMap::addSegment(const cg3::Segment2d& segment) {
    // Find the faces in the trapezoidal map T that intersect the segment
    // sorted from left to right
        ///TODO D.tmp();
    OrderedSegment* orderedSegment = new OrderedSegment(segment);
    auto facesIntersected = std::vector<Trapezoid*>();
    followSegment(*orderedSegment , facesIntersected);
    // Split those faces and update the map/dag with the new faces
    split(*orderedSegment , facesIntersected);
}

Trapezoid* TrapezoidalMap::pointLocation(cg3::Point2d pointToQuery) {
    return D.query(pointToQuery);
}

void TrapezoidalMap::clear() {
    D.clear();
    for (auto it=T.begin(); it != T.end(); it++)
        delete *it;
    T.clear();
}
// ----------------------- END PUBLIC SECTION -----------------------




// ----------------------- PRIVATE SECTION -----------------------
void TrapezoidalMap::followSegment(OrderedSegment& s, std::vector<Trapezoid*>& facesIntersectingSegment) {
    // 1. Let p and q be the left and right endpoint of s.
    auto p = s.getLeftmost();
    auto q = s.getRightmost();
    // 2. Search with p in the search structure D to find d0.
    Trapezoid* currentFace = D.query(p);

    facesIntersectingSegment.push_back(currentFace);
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
        if(currentFace != nullptr)
            facesIntersectingSegment.push_back(currentFace);
    }
//    if(tmp != nullptr && tmp != currentFace && tmp->getLeftp().x()<=q.x()&&q.x()<=tmp->getRightp().x())
//        facesIntersectingSegment.push_back(tmp);
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
//        topNewFace.setLowerLeftNeighbor(onlyFace->getLowerLeftNeighbor()); does the LL exist?
        topNewFace->setUpperRightNeighbor(rightNewFace);
//        topNewFace.setLowerRightNeighbor(bottomNewFace); does the LR exist?

        // right
        rightNewFace->setUpperLeftNeighbor(topNewFace);
        rightNewFace->setLowerLeftNeighbor(bottomNewFace);
        rightNewFace->replaceNeighborsFromTrapezoid(oldFace, {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

        // bottom
        bottomNewFace->setLowerLeftNeighbor(leftNewFace);
        bottomNewFace->setLowerRightNeighbor(rightNewFace);

        //TODO      do UR and UL exist?

        // Add the new faces on the trapezoidal map
        T.push_back(leftNewFace);
        T.push_back(topNewFace);
        T.push_back(bottomNewFace);
        T.push_back(rightNewFace);

        // Upgrade the DAG
        D.replaceNodeWithSubtree(oldFace->getPointerToDAG(), s, leftNewFace, topNewFace, bottomNewFace, rightNewFace);

        // Remove the old one from T
        auto d = T.size();
        auto tmp = intersectingFaces.front();
        intersectingFaces.clear();
        T.erase(std::remove(T.begin(), T.end(), tmp), T.end());;
        delete tmp;
        assert(intersectingFaces.empty() == true);
        assert(T.size() == (d-1));
    }

    /* HARDEST CASE */
    else {
        Trapezoid* firstFace, *lastFace;
        std::vector<Trapezoid*> aboveSegmentNewFaces(N_FACES+2);
        std::vector<Trapezoid*> belowSegmentNewFaces(N_FACES+2);
        // source: https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
        std::vector<std::vector<Trapezoid*>> finalNewFaces(2, std::vector <Trapezoid*> (N_FACES, nullptr));

        /* SPLIT THE OLD FACES */
        for(size_t i = 0; i<N_FACES; i++){
            Trapezoid* oldFace = intersectingFaces.at(i);
            // first face
            if(i==0){
                /* split it into three parts: leftNewFace, topNewFace, bottomNewFace*/
                // left
                firstFace = new Trapezoid(oldFace->getTop(), oldFace->getBottom(), oldFace->getLeftp(), s.getLeftmost());
                firstFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPLEFT, Trapezoid::BOTTOMLEFT});
                // top
                Trapezoid* topNewFace = new Trapezoid(oldFace->getTop(), s, s.getLeftmost(), oldFace->getRightp());
                // bottom
                Trapezoid* bottomNewFace = new Trapezoid(s, oldFace->getBottom(), s.getLeftmost(), oldFace->getRightp());

                //
                aboveSegmentNewFaces.at(i)=  firstFace;
                belowSegmentNewFaces.at(i) = firstFace;

                aboveSegmentNewFaces.at(i+1) = topNewFace;
                belowSegmentNewFaces.at(i+1) = bottomNewFace;
            }
            // last face
            else if(i==intersectingFaces.size()-1) {
                /* split it into three parts: topNewFace, bottomNewFace, rightNewFace*/
                // right
                lastFace = new Trapezoid(oldFace->getTop(), oldFace->getBottom(), s.getRightmost(), oldFace->getRightp());
                lastFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i), {Trapezoid::TOPRIGHT, Trapezoid::BOTTOMRIGHT});

                // top
                Trapezoid* topNewFace = new Trapezoid(oldFace->getTop(), s, oldFace->getLeftp(), s.getRightmost());
                // bottom
                Trapezoid* bottomNewFace = new Trapezoid(s, oldFace->getBottom(), oldFace->getLeftp(), s.getRightmost());

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
        for(auto i = 1; i < aboveSegmentNewFaces.size()-1; i++) {
            /* ABOVE */
            Trapezoid* tmpTopFace = aboveSegmentNewFaces.at(i);

            // LowerRight
            tmpTopFace->setLowerRightNeighbor(aboveSegmentNewFaces.at(i+1));
            // LowerLeft
            tmpTopFace->setLowerLeftNeighbor(aboveSegmentNewFaces.at(i-1));
            // UpperLeft and UpperRight
            tmpTopFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::TOPLEFT, Trapezoid::TOPRIGHT});

            /* BELOW */
            Trapezoid* tmpBottomFace = belowSegmentNewFaces.at(i);

            // UpperRight
            tmpBottomFace->setUpperRightNeighbor(aboveSegmentNewFaces.at(i+1));
            // UpperLeft
            tmpBottomFace->setUpperLeftNeighbor(aboveSegmentNewFaces.at(i-1));
            // LowerLeft and LowerRight
            tmpBottomFace->replaceNeighborsFromTrapezoid(intersectingFaces.at(i-1), {Trapezoid::BOTTOMLEFT, Trapezoid::BOTTOMRIGHT});
        }

        // Merging the eventually adjacent faces with same bottom and top
        /* ABOVE */
        T.push_back(firstFace);
        for(auto i = 1; i < N_FACES+1; i++) {
            int j = i;
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

                for(auto k=i; k<=j; k++)
                    finalNewFaces[0].at(k-1) = mergedFace;

                // Jump to the face after Fj
                i = j;
                T.push_back(mergedFace);
            }
            // Otherwise
            else {
                finalNewFaces[0].at(i-1) = aboveSegmentNewFaces.at(i);
                T.push_back(finalNewFaces[0].at(i-1));
            }
        }

        /* BELOW */
        for(auto i = 1; i < N_FACES+1; i++) {
            int j = i;
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

                for(auto k=i; k<=j; k++)
                    finalNewFaces[1].at(k-1) = mergedFace;

                T.push_back(mergedFace);
                // Jump to the face after Fj
                i = j;
            }
            // Otherwise
            else {
                finalNewFaces[1].at(i-1) = belowSegmentNewFaces.at(i);
                T.push_back(finalNewFaces[1].at(i-1));
            }
        }
        T.push_back(lastFace);

        // Update the DAG
        for(auto i = 0; i < N_FACES; i++) {
            if(i==0) {
                D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, firstFace, finalNewFaces[0].at(i), finalNewFaces[1].at(i), nullptr);
            } else if (i == N_FACES-1) {
                D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, nullptr, finalNewFaces[0].at(i), finalNewFaces[1].at(i), nullptr);
            } else {
                D.replaceNodeWithSubtree(intersectingFaces.at(i)->getPointerToDAG(), s, nullptr, finalNewFaces[0].at(i), finalNewFaces[1].at(i), lastFace);
            }
        }

        // Clean
        // TODO To improve
        for(auto f : intersectingFaces) {
            auto tmp = f;
//            intersectingFaces.clear();
            T.erase(std::remove(T.begin(), T.end(), tmp), T.end());;
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
