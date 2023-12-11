#ifndef ECC_H
#define ECC_H
#include <bits/stdc++.h>
#include "FiniteFieldElement.hpp"
#include "Point.hpp"	
#include "BigNum.hpp"
class ECC{
    private:
        BigNum mod_prime;
        FiniteFieldElement a, b;
        Point gen;
        BigNum order;
        
        // FiniteFieldElement toElement(const Point&);
    public:
        const std::vector<Point> toPoint(const FiniteFieldElement&);
        ECC(BigNum& mod_prime, FiniteFieldElement& a, FiniteFieldElement& b, Point& gen , BigNum& order);
        const bool isOnCurve(const Point&);
        const bool isZero(const Point&);
        const Point addition(const Point&, const Point&);
        const Point negate(const Point&);
        const Point doublePoint(const Point&);
        const Point multiply(const Point&,const BigNum&);

        const Point embed(std::string&);
        const std::string unembed(Point&);
        const Point decode(std::string&);
        const std::string encode(const Point&);

        const std::vector<Point> encrypt(Point&, BigNum&, Point&);
        const Point decrypt(std::vector<Point>&, BigNum&);

        friend std::ostream& operator<<(std::ostream&, const ECC&);

};
#endif