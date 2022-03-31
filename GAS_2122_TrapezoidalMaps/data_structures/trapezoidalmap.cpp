#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap(cg3::BoundingBox2& B)
{
    // 1. Initialize the trapezoidal map structure T and search structure D
    D(B);
}
