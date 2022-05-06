#ifndef DRAWABLETRAPEZOID_H
#define DRAWABLETRAPEZOID_H

#include "data_structures/trapezoid.h"

class DrawableTrapezoid : public Trapezoid
{
public:
    DrawableTrapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp, bool doCalculateGraphics = false);

    void calculateGraphics();
    bool isGraphicsCalculated();
    // draw methods
    void draw() const;
    // getter/setter
    static double getYMin();
    static void setYMin(double newYMin);
    static double getYMax();
    static void setYMax(double newYMax);


    static double yMin, yMax;
    cg3::Point2d topLeftVertex, topRightVertex, bottomLeftVertex, bottomRightVertex;
    cg3::Color polygonColor = cg3::Color(0.0, 0.0, 0.0);

    bool isHighlighted = false;

private:
    void setRandomColor() ;
    void setVerteces() ;

    bool hasGraphics = false;
    cg3::Color segmentColor = cg3::Color(80.0, 80.0, 180.0);
    int segmentSize = 3;
};

#endif // DRAWABLETRAPEZOID_H
