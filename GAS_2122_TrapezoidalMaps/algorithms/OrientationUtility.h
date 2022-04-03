#ifndef ORIENTATIONUTILITY_H
#define ORIENTATIONUTILITY_H

#include "cg3/geometry/point2.h"
#include "data_structures/orderedsegment.h"
//#include "eigen3/signature_of_eigen3_matrix_library"

enum Position {left, center, right};
double det3(double m[3][3]);
Position getPointPositionRespectToLine(cg3::Point2& p, OrderedSegment& s);

#endif // ORIENTATIONUTILITY_H
