#include "OrientationUtility.h"

namespace OrientationUtility {
    double det3(double m[3][3])
    {
        // (AEI + BFG + CDH) - (AFH + BDI + CEG)
        //  A   B   C
        //  D   E   F
        //  G   H   I
        double a,b,c,d,e,f,g,h,i;
        a = m[0][0];
        b = m[0][1];
        c = m[0][2];
        d = m[1][0];
        e = m[1][1];
        f = m[1][2];
        g = m[2][0];
        h = m[2][1];
        i = m[2][2];
        return (a*e*i + b*f*g + c*d*h) - (a*f*h + b*d*i + c*e*g);
    }

    Position getPointPositionRespectToLine(cg3::Point2& p, OrderedSegment& s) {
        double m[3][3] = {
            {s.getLeftmost().x(), s.getLeftmost().y(),1.0},
            {s.getRightmost().x(), s.getRightmost().y(),1.0},
            {p.x(), p.y(), 1.0}
        };
        double determinant = det3(m);

        if (determinant < 0)
            return right;
        else if (determinant > 0)
            return left;
        return center;
    }
}
