#include "drawabletrapezoidalmap.h"

DrawableTrapezoidalMap::DrawableTrapezoidalMap()
{

}

void DrawableTrapezoidalMap::initialize(const cg3::BoundingBox2& B) {
    TrapezoidalMap::initialize(B);
    setBoundingBox(B);
}

void DrawableTrapezoidalMap::draw() const
{
    // For each trapezoid
    for(auto t : T) {
        /* DRAW THE TRAPEZOID */
        /*source: https://www3.ntu.edu.sg/home/ehchua/programming/opengl/cg_introduction.html*/
        glBegin(GL_POLYGON);            // These vertices form a closed polygon
            if(t->isHighlighted) {
                glColor3f(1.0f, 0.0f, 0.0f);
            } else {
                ///TODO to change
                glColor3f(t->color.redF(), t->color.greenF(), t->color.blueF());
            }
            glVertex2f(t->topLeftVertex.x(),     t->topLeftVertex.y());
            glVertex2f(t->topRightVertex.x(),    t->topRightVertex.y());
            glVertex2f(t->bottomRightVertex.x(), t->bottomRightVertex.y());
            glVertex2f(t->bottomLeftVertex.x(),  t->bottomLeftVertex.y());
         glEnd();

     /* DRAW THE VERTICAL LINES*/
     cg3::Color c = cg3::Color(0,0,0);
     cg3::opengl::drawLine2(t->topLeftVertex, t->bottomLeftVertex, c, 2);
     cg3::opengl::drawLine2(t->topRightVertex, t->bottomRightVertex, c, 2);
     //TODO To be continued
    }
}

//TODO Should be equal to the implementation in trapezoidalmap_dataset (?)
cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

//TODO Should be equal to the implementation in trapezoidalmap_dataset (?)
double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return boundingBox.diag();
}

const cg3::BoundingBox2 &DrawableTrapezoidalMap::getBoundingBox() const
{
    return B;
}

void DrawableTrapezoidalMap::setBoundingBox(const cg3::BoundingBox2 &newB)
{
    B = newB;
}

//Trapezoid *DrawableTrapezoidalMap::getLastTrapezoidHighlighted() const
//{
//    return lastTrapezoidHighlighted;
//}

void DrawableTrapezoidalMap::highlightTrapezoid(Trapezoid *newLastTrapezoidHighlighted)
{
    if(lastTrapezoidHighlighted != nullptr)
        lastTrapezoidHighlighted->isHighlighted = false;

    newLastTrapezoidHighlighted->isHighlighted = true;
    lastTrapezoidHighlighted = newLastTrapezoidHighlighted;
}
