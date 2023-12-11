#ifndef POINT_H
#define POINT_H

#include <bits/stdc++.h>
#include "BigNum.hpp"
#include "FiniteFieldElement.hpp"

class Point{
    public:
    FiniteFieldElement x, y;

    Point();
    Point(const FiniteFieldElement &,const  FiniteFieldElement &);

    friend bool operator==(const Point&, const Point&);
    friend bool operator!=(const Point&, const Point&);

};

#endif