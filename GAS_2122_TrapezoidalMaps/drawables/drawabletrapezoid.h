#ifndef DRAWABLETRAPEZOID_H
#define DRAWABLETRAPEZOID_H

#include "data_structures/trapezoid.h"

class DrawableTrapezoid : public Trapezoid
{
public:
    DrawableTrapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp, bool doCalculateGraphics = false);

    void calculateGraphics();
    bool isGraphicsCalculated() const;
    // draw methods
    void drawPolygon() const;
    void drawVerticalLines() const;

    // getter/setter
    static double getYMin();
    static void setYMin(double newYMin);
    static double getYMax();
    static void setYMax(double newYMax);


    static double yMin, yMax;

    bool getIsHighlighted() const;
    void setIsHighlighted(bool newIsHighlighted);

private:
    void setRandomColor() ;
    void setVerteces() ;

    // flags
    bool hasGraphics = false;   // if the graphics staff has already calculated or not
    bool isHighlighted = false; // if the face is highlighted or not (it will change the color)

    // 4 verteces of the trapezoid
    cg3::Point2d topLeftVertex, topRightVertex, bottomLeftVertex, bottomRightVertex;

    // Colors and size for drawing
    cg3::Color polygonColor = cg3::Color(0.0, 0.0, 0.0);
    cg3::Color segmentColor = cg3::Color(80.0, 80.0, 180.0);
    int segmentSize = 3;
};

#endif // DRAWABLETRAPEZOID_H
