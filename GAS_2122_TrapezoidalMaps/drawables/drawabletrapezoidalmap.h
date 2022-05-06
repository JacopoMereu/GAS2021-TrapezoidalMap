#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"
#include "drawabletrapezoid.h"
#include <cg3/viewer/interfaces/drawable_object.h>

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap();

    // override of the virtual methods from DrawableObject
    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;

    // Override trapezoidalmap
    void initialize(const cg3::BoundingBox2& B);

    // other methods
    void highlightTrapezoid(DrawableTrapezoid* newLastTrapezoidHighlighted);
    void resetLastTrapezoidHighlighted();

private:
    DrawableTrapezoid* lastTrapezoidHighlighted = nullptr;

//    Trapezoid *getLastTrapezoidHighlighted() const;
};

#endif // DRAWABLETRAPEZOIDALMAP_H
