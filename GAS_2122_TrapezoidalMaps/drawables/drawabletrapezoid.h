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

    // COLORS
    const double min_random_value = 0.0;
    const double max_random_value = 0.8;
    // polygon highlighted color (equal for all trapezoids)
    const cg3::Color POLYGON_COLOR_WHEN_HIGHLIGHTED = cg3::Color(255,255,255);
    // polygon default color
    cg3::Color polygonColor = cg3::Color(0, 0, 0);
    // vertical lines
    cg3::Color segmentColor = cg3::Color(80, 80, 180); // value seen in drawableboundingbox, but it has not getter, so I set this field "manually"
    // WIDTH
    int segmentSize = 3; // value seen in drawableboundingbox, but it has not getter, so I set this field "manually"


};

#endif // DRAWABLETRAPEZOID_H
