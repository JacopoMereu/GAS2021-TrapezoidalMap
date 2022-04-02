#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include "dag.h"
#include "trapezoid.h"
#include <set>

class TrapezoidalMap
{
public:
    TrapezoidalMap::TrapezoidalMap(cg3::BoundingBox2& B);

private:
    DAG D;
    std::set<Trapezoid> Map;
};

#endif // TRAPEZOIDALMAP_H
