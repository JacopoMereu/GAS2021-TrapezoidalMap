#include "drawabletrapezoidalmap.h"
#include <cg3/viewer/opengl_objects/opengl_objects2.h>

DrawableTrapezoidalMap::DrawableTrapezoidalMap() {}

/// Override DrawableObject
// Draw the objects through opengl calls
void DrawableTrapezoidalMap::draw() const
{
    // For each trapezoid in the map
    for(auto t : T) {
        /* DRAW ITS VERTICAL LINES*/
        t->drawVerticalLines();

        /* DRAW THE POLYGON */
        t->drawPolygon(); // IT MUST BE DONE AFTER THE VERTICAL LINES, OTHERWISE THE LINES WON'T BE VISIBLE
    }
}

// See drawable_trapezoidalmap_dataset.cpp
cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

// See drawable_trapezoidalmap_dataset.cpp
double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return boundingBox.diag();
}

/// Override TrapezoidalMap
void DrawableTrapezoidalMap::initialize(const cg3::BoundingBox2 &B) {
    // set the minimum y-coordinate and the maximum y-coordinate of the boundarybox in the drawable trapezoid class
    auto yMin = B.min().y();
    auto yMax = B.max().y();
    DrawableTrapezoid::setYMax(yMax);
    DrawableTrapezoid::setYMin(yMin);

    // parent call
    TrapezoidalMap::initialize(B);
}

/// Others
void DrawableTrapezoidalMap::highlightTrapezoid(DrawableTrapezoid *newLastTrapezoidHighlighted)
{
    if(newLastTrapezoidHighlighted == nullptr) return;

    newLastTrapezoidHighlighted->setIsHighlighted(true);
    lastTrapezoidHighlighted = newLastTrapezoidHighlighted;
}

void DrawableTrapezoidalMap::resetLastTrapezoidHighlighted() {
    if(lastTrapezoidHighlighted == nullptr) return;

    lastTrapezoidHighlighted->setIsHighlighted(false);
}
