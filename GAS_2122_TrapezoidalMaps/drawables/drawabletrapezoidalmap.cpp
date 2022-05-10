#include "drawabletrapezoidalmap.h"
#include <cg3/viewer/opengl_objects/opengl_objects2.h>

DrawableTrapezoidalMap::DrawableTrapezoidalMap()
{

}

// Override DrawableObject
void DrawableTrapezoidalMap::draw() const
{
    for(auto t : T) {
        /* DRAW THE VERTICAL LINES*/
        t->drawVerticalLines();

        /* DRAW THE POLYGON */
        t->drawPolygon(); // IT MUST BE DONE AFTER THE VERTICAL LINES
    }
}

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return boundingBox.diag();
}

// trapezoidalmap
void DrawableTrapezoidalMap::initialize(const cg3::BoundingBox2 &B) {
    auto yMin = B.min().y();
    auto yMax = B.max().y();
    DrawableTrapezoid::setYMax(yMax);
    DrawableTrapezoid::setYMin(yMin);

    TrapezoidalMap::initialize(B);
}
// Others
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
