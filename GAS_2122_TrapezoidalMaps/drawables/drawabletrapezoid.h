#ifndef DRAWABLETRAPEZOID_H
#define DRAWABLETRAPEZOID_H

#include "data_structures/trapezoid.h"

class DrawableTrapezoid : public Trapezoid
{
public:
    // Constructor similar to the parent one: the only plus is that
    //      we can choose to calculate or not the graphics. The default value is false.
    DrawableTrapezoid(const OrderedSegment& t, const OrderedSegment& b, const cg3::Point2d& lp, const cg3::Point2d& rp, bool doCalculateGraphics = false);

    // choose a random color for the trapezoid and calculate its four verteces necessary for drawing the polygon
    void calculateGraphics();
    // return true if the graphics has already been calulcated, false otherwise
    bool isGraphicsCalculated() const;


    ////////////////// DRAW METHODS //////////////////
    // draw the trapezoid through opengl calls
    void drawPolygon() const;
    // draw the vertical lines of the trapezoid through cg3 calls (which use opengl methods)
    void drawVerticalLines() const;
    //////////////////////////////////////////////////



    /////////////////// GETTER ///////////////////
    // Get the minimum y-coordinate of the bounding box
    static double getYMin();
    // Get the maximum y-coordinate of the bounding box
    static double getYMax();

    // return true if this trapezoid is highlighted, false otherwise
    bool getIsHighlighted() const;
    //////////////////////////////////////////////


    /////////////////// SETTER ///////////////////
    // Set the minimum y-coordinate of the bounding box with the double take in input
    static void setYMin(double newYMin);
    // Set the maximum y-coordinate of the bounding box with the double take in input
    static void setYMax(double newYMax);

    // Set the highlighted status with the boolean taken in input
    void setIsHighlighted(bool newIsHighlighted);
    //////////////////////////////////////////////

    // static variables that rapresent the minimum y-coordinate of the boundary and the maximum y-coordinate of the boundary.
    // These two are essential for calculating the 4 verteces of the trapezoid.
    static double yMin, yMax;

private:
    // set a random color for this trapezoid
    void setRandomColor() ;
    // calculate the four verteces of the trapezoid
    void setVerteces() ;

    /// flags
    bool hasGraphics = false;   // if the graphics stuff has already been calculated or not
    bool isHighlighted = false; // if the face is highlighted or not (it will change the color)

    // The 4 verteces of the trapezoid
    cg3::Point2d topLeftVertex, topRightVertex, bottomLeftVertex, bottomRightVertex;

    /// COLORS & WIDTH
    // min and max value for the uniform distribution used to choose a random color
    const double min_random_value = 0.0;
    const double max_random_value = 0.8;

    // polygon highlighted color (equal for all trapezoids)
    const cg3::Color POLYGON_COLOR_WHEN_HIGHLIGHTED = cg3::Color(255,255,255);
    // polygon default color
    cg3::Color polygonColor = cg3::Color(0, 0, 0);

    // vertical lines colors
    cg3::Color segmentColor = cg3::Color(80, 80, 180); // value seen in drawableboundingbox, but it has not getter, so I set this field "manually"
    // size of the lines
    int segmentSize = 3; // value seen in drawableboundingbox, but it has not getter, so I set this field "manually"
};

#endif // DRAWABLETRAPEZOID_H
