#include "trapezoidalmap.h"

// ----------------------- PUBLIC SECTION -----------------------

TrapezoidalMap::TrapezoidalMap(cg3::BoundingBox2 B)
{

    // Adding the verteces of the BB
    cg3::Point2 topleft = cg3::Point2(B.min()+B.lengthY());
    cg3::Point2 topright = cg3::Point2(B.max());
    cg3::Point2 bottomleft = cg3::Point2(B.min());
    cg3::Point2 bottomright = cg3::Point2(B.min()+B.lengthX());

    // Create a trapezoid from the bounding box
    cg3::Segment2 top = cg3::Segment(topleft, topright);
    cg3::Segment2 bottom = cg3::Segment(bottomleft, bottomright);
    Trapezoid boundingbox_trapezoid = Trapezoid(top, bottom, bottomleft, topright);

    // Initialize the trapezoidal map structure T and search structure D
    T = std::set<Trapezoid>(boundingbox_trapezoid);
    D = DAG(&boundingbox_trapezoid);
}


void TrapezoidalMap::addSegment(cg3::Segment2& segment) {
    // 1) Find the faces in the trapezoidal map T that intersect the segment
    std::vector<Trapezoid> facesIntersected = getFacesIntersectingSegment(segment);

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
std::vector<Trapezoid&> TrapezoidalMap::getFacesIntersectingSegment(cg3::Segment s) {

}

void TrapezoidalMap::splitFaceIntersectingSegment(Trapezoid& face, cg3::Segment s, std::set<Trapezoid>& newFaces) {

}

// ----------------------- END PRIVATE SECTION -----------------------
