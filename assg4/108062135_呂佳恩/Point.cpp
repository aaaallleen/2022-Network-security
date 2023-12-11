#include "Point.hpp"
using namespace std;
Point::Point():x(0),y(0){ }
Point::Point(const FiniteFieldElement &x,const FiniteFieldElement &y) :x(x),y(y) { }
bool operator==(const Point &lhs, const Point &rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
bool operator!=(const Point &lhs, const Point &rhs) {
    return !(lhs == rhs);
}
