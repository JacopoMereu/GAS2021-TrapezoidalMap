#ifndef ORIENTATIONUTILITY_H
#define ORIENTATIONUTILITY_H

#include "cg3/geometry/point2.h"
#include "data_structures/orderedsegment.h"
//#include "eigen3/signature_of_eigen3_matrix_library"


enum Position {above, middle, below};
namespace OrientationUtility {
    double det3(double m[3][3]);
    Position getPointPositionRespectToLine(const cg3::Point2d& p, const OrderedSegment& s);
}
#endif // ORIENTATIONUTILITY_H
