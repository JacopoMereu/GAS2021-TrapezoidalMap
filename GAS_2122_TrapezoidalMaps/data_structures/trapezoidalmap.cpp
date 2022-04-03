#include "trapezoidalmap.h"

// ----------------------- PUBLIC SECTION -----------------------

TrapezoidalMap::TrapezoidalMap(cg3::BoundingBox2 B)
{

    // Adding the verteces of the BB
    auto topleft     = cg3::Point2(B.min()+B.lengthY());
    auto topright    = cg3::Point2(B.max());
    auto bottomleft  = cg3::Point2(B.min());
    auto bottomright = cg3::Point2(B.min()+B.lengthX());

    // Create a trapezoid from the bounding box
    auto top = OrderedSegment(topleft, topright);
    auto bottom = OrderedSegment(bottomleft, bottomright);
    auto boundingbox_trapezoid = Trapezoid(top, bottom, bottomleft, topright);

    // Initialize the trapezoidal map structure T and search structure D
    T = std::set<Trapezoid>(boundingbox_trapezoid);
    D = DAG(&boundingbox_trapezoid);
}


void TrapezoidalMap::addSegment(OrderedSegment& segment) {
    // 1) Find the faces in the trapezoidal map T that intersect the segment
    auto facesIntersected = std::vector<Trapezoid&>();
    followSegment(segment, facesIntersected);

    // 2) Remove the faces found from T and replace them by the new trapezoids:
    // 2a) Split the faces
    auto newFaces = std::vector<Trapezoid>();
    for (auto face : facesIntersected) {
        // split the i-th face in 2, 3 or 4 faces
        splitFaceIntersectingSegment(face, segment, newFaces_tmp);
        //add the new faces to the set of current face
        newFaces.insert(newFaces.end(), newFaces_tmp.begin(), newFaces_tmp.end());
    }
    // 2b) Remove the old faces
    T.erase(facesIntersected.begin(), facesIntersected.end());
    // 2c) Add the new ones
    T.insert(newFaces.begin(), newFaces.end());

    // 3) Remove the faces from D, and create leaves for the new trapezoids.
    //  Link the new leaves to the existing inner nodes by adding some new inner nodes.
    //TODO    D.upgrade();
}
// ----------------------- END PUBLIC SECTION -----------------------




// ----------------------- PRIVATE SECTION -----------------------
void TrapezoidalMap::followSegment(OrderedSegment& s, std::vector<Trapezoid&> facesIntersectingSegment) {
    // 1. Let p and q be the left and right endpoint of s.
    auto p = s.getLeftmost();
    auto q = s.getRightmost();
    // 2. Search with p in the search structure D to find d0.
    //TODO Trapezoid currentFace = queryInDAG()

    Trapezoid currentFace;
    facesIntersectingSegment.push_back(currentFace);

    //  while q lies to the right of rightp(dj)
    while(q > currentFace.getRightp()) {
        // if rightp(dj) lies above the segment
        if(getPointPositionRespectToLine(currentFace, s) == left) {
            currentFace = currentFace.getLowerRightNeighbor();
        } else {
            currentFace = currentFace.getLowerLeftNeighbor();
        }

        //
        facesIntersectingSegment.push_back(currentFace);
    }
}

void TrapezoidalMap::splitFaceIntersectingSegment(Trapezoid& face, OrderedSegment s, std::set<Trapezoid>& newFaces) {

}

// ----------------------- END PRIVATE SECTION -----------------------
