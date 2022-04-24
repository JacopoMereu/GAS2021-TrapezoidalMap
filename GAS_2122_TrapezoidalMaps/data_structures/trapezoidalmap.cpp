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
#if 0
    // 2) Remove the faces found from T and replace them by the new trapezoids:
    // 2a) Split the faces
    auto newFaces = std::vector<Trapezoid>();
    for (auto face : facesIntersected) {
        // split the i-th face in 2, 3 or 4 faces
        splitFaceIntersectingSegment(face, segment, newFaces_tmp);
        //add the new faces to the set of current face
        newFaces.insert(newFaces.end(), newFaces_tmp.begin(), newFaces_tmp.end());

        // replace the node (in the DAG) containing the i-th face with the splitted ones.
        D.replaceNodeWithSubtree(face.getPointerToDAG(), segment, newFaces);
    }
    // 2b) Remove the old faces
    T.erase(facesIntersected.begin(), facesIntersected.end());
    // 2c) Add the new ones
    T.insert(newFaces.begin(), newFaces.end());

    // 3) Remove the faces from D, and create leaves for the new trapezoids.
    //  Link the new leaves to the existing inner nodes by adding some new inner nodes.
#else
    /* EASIEST CASE */
    if(intersectingFaces.size() == 1) {
        Trapezoid* onlyFace = intersectingFaces[0];

        // left
        Trapezoid leftNewFace = Trapezoid(onlyFace->getTop(), onlyFace->getBottom(), onlyFace->getLeftp(), s.getLeftmost());
        // top
        Trapezoid topNewFace = Trapezoid(onlyFace->getTop(), s, s.getLeftmost(), s.getRightmost());
        // right
        Trapezoid rightNewFace = Trapezoid(onlyFace->getTop(), onlyFace->getBottom(), s.getRightmost(), onlyFace->getRightp());
        // bottom
        Trapezoid bottomNewFace = Trapezoid(s, onlyFace->getBottom(), s.getLeftmost(), s.getRightmost());

        // setting adjacencies for:
        // left
        leftNewFace.setUpperLeftNeighbor(onlyFace->getUpperLeftNeighbor());
        leftNewFace.setLowerLeftNeighbor(onlyFace->getLowerLeftNeighbor());
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
        rightNewFace.setUpperRightNeighbor(onlyFace->getUpperRightNeighbor());
        rightNewFace.setLowerRightNeighbor(onlyFace->getLowerRightNeighbor());

        // bottom
        bottomNewFace.setLowerLeftNeighbor(&leftNewFace);
        bottomNewFace.setLowerRightNeighbor(&rightNewFace);
        //TODO      do UR and UL exist?

        // Update the faces (if not null) that had the old face as neighbor
        if(onlyFace->getUpperLeftNeighbor())
            onlyFace->getUpperLeftNeighbor()->replaceNeighbor(onlyFace, &leftNewFace);
        if(onlyFace->getLowerLeftNeighbor())
            onlyFace->getLowerLeftNeighbor()->replaceNeighbor(onlyFace, &leftNewFace);
        if(onlyFace->getUpperRightNeighbor())
            onlyFace->getUpperRightNeighbor()->replaceNeighbor(onlyFace, &rightNewFace);
        if(onlyFace->getLowerRightNeighbor())
            onlyFace->getLowerRightNeighbor()->replaceNeighbor(onlyFace, &rightNewFace);

        // Add the new faces on the trapezoidal map
         std::vector<Trapezoid*> tmpNewFaces = {&leftNewFace, &topNewFace, &rightNewFace, &bottomNewFace};
        assert(tmpNewFaces.size() == 4);
        /*T.push_back(&leftNewFace);
        T.push_back(&topNewFace);
        T.push_back(&rightNewFace);
        T.push_back(&bottomNewFace);*/
        T.insert(T.end(), tmpNewFaces.begin(), tmpNewFaces.end());

        // Upgrade the DAG
        D.replaceNodeWithSubtree(onlyFace->getPointerToDAG(), s, tmpNewFaces);

        // Remove the old one from T
        auto d = T.size();
        T.erase(intersectingFaces.begin());
        assert(intersectingFaces.empty() == true);
        assert(T.size() == (d-1));
    }
    /* HARDEST CASE */
    else {

    }
#endif
    /**/
}


// ----------------------- END PRIVATE SECTION -----------------------
