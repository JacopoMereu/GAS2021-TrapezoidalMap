#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include "data_structures/trapezoidalmap.h"
#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/viewer/opengl_objects/opengl_objects2.h>

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap();

    // override of the virtual methods
    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;

    // other methods
    void initialize(const cg3::BoundingBox2& B);

    const cg3::BoundingBox2 &getBoundingBox() const;
    void setBoundingBox(const cg3::BoundingBox2 &newB);

private:
    cg3::BoundingBox2 B;

};

#endif // DRAWABLETRAPEZOIDALMAP_H
