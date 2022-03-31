#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "dag.h"
class TrapezoidalMap
{
public:
    TrapezoidalMap::TrapezoidalMap(cg3::BoundingBox2& B);

private:
    DAG D;

};

#endif // TRAPEZOIDALMAP_H
