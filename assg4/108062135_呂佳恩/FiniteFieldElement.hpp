#ifndef FINITEFIELD_H
#define FINITEFIELD_H
#include <bits/stdc++.h>
#include "BigNum.hpp"

class FiniteFieldElement{
    private:
        
    public:
        static BigNum mod_prime;
        BigNum i;
    	
        FiniteFieldElement();
        FiniteFieldElement(long long);
        FiniteFieldElement(const std::string&);
        FiniteFieldElement(BigNum);
        //define comparison operands
        friend bool operator==(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend bool operator!=(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend bool operator>(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend bool operator<(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend bool operator<=(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend bool operator>=(const FiniteFieldElement&,  const FiniteFieldElement&);
        //define calculations operands
        friend FiniteFieldElement operator+(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend FiniteFieldElement operator-(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend FiniteFieldElement operator*(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend FiniteFieldElement operator/(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend FiniteFieldElement operator%(const FiniteFieldElement&,  const FiniteFieldElement&);
        friend FiniteFieldElement operator-(const FiniteFieldElement&);

        friend FiniteFieldElement inv(const FiniteFieldElement&);
        friend std::vector<FiniteFieldElement> sqrt(FiniteFieldElement&);
        friend FiniteFieldElement pow(FiniteFieldElement, BigNum );
        friend std::ostream& operator<<(std::ostream&, FiniteFieldElement&);
        std::string to_string() const;
        std::vector<BigNum> EEA(BigNum, BigNum) const;
};
#endif
