#include "drawabletrapezoidalmap.h"

DrawableTrapezoidalMap::DrawableTrapezoidalMap()
{

}

// Override DrawableObject
void DrawableTrapezoidalMap::draw() const
{
    // For each trapezoid
    for(auto t : T) {
        /* DRAW THE TRAPEZOID */
        t->draw();

     /* DRAW THE VERTICAL LINES*/

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

    newLastTrapezoidHighlighted->isHighlighted = true;
    lastTrapezoidHighlighted = newLastTrapezoidHighlighted;
}

void DrawableTrapezoidalMap::resetLastTrapezoidHighlighted() {
    if(lastTrapezoidHighlighted == nullptr) return;

    lastTrapezoidHighlighted->isHighlighted = false;
}
