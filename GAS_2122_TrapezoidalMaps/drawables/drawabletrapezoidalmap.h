#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"
#include "drawabletrapezoid.h"
#include <cg3/viewer/interfaces/drawable_object.h>

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap();

    /// Override of the virtual methods from DrawableObject: see the documentation of drawable_object
    // draw all the trapezoids contained in the trapezoidal map
    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;

    /// Override the virtual method from Trapezoidalmap
    void initialize(const cg3::BoundingBox2& B);

    /// other methods
    // set the trapezoid given in input as "highlighted", only if it's not null
    void highlightTrapezoid(DrawableTrapezoid* newLastTrapezoidHighlighted);
    // set the last trapezoid as "not highlighted"
    void resetLastTrapezoidHighlighted();

private:
    // the pointer to the last trapezoid highlighted
    DrawableTrapezoid* lastTrapezoidHighlighted = nullptr;
};

#endif // DRAWABLETRAPEZOIDALMAP_H
