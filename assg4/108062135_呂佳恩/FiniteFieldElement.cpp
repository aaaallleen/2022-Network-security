#include "FiniteFieldElement.hpp"
using namespace std;
BigNum FiniteFieldElement::mod_prime = 1;

vector<BigNum> FiniteFieldElement::EEA(BigNum a, BigNum b) const{
    BigNum q, x = 0, y = 1, lx = 1, ly = 0, tmp1, tmp2, tmp3;
    while(b!=0){
        q = a/b;
        tmp1 = a%b;
        a = b;
        b = tmp1;
        tmp2 = x;
        x = lx-q*x;
        lx = tmp2;
        tmp3 = y;
        y = ly-q*y;
        ly = tmp3;
    }
    vector<BigNum> result = {lx, ly};
    return result;
    
}

FiniteFieldElement::FiniteFieldElement() : i(0) { }
FiniteFieldElement::FiniteFieldElement(BigNum i){ 
    this->i = i % mod_prime;
} 
FiniteFieldElement::FiniteFieldElement(const string& s){
    this->i = BigNum(s) % mod_prime;
}
FiniteFieldElement::FiniteFieldElement(long long i){
    this->i = i % mod_prime;
}
//overload comparison operators, compare the i values
bool operator==(const FiniteFieldElement& lhs,const FiniteFieldElement& rhs){
    return lhs.i == rhs.i;
}
bool operator!=(const FiniteFieldElement& lhs,const FiniteFieldElement& rhs){
    return lhs.i != rhs.i;
}
bool operator<(const FiniteFieldElement& lhs,const FiniteFieldElement& rhs){
    return lhs.i < rhs.i;
}
bool operator>(const FiniteFieldElement& lhs,const FiniteFieldElement& rhs){
    return lhs.i > rhs.i;
}
bool operator<=(const FiniteFieldElement& lhs,const FiniteFieldElement& rhs){      
    return lhs.i <= rhs.i;
}
bool operator>=(const FiniteFieldElement& lhs,const FiniteFieldElement& rhs){
    return lhs.i >= rhs.i;
}
//overload the calculation operators, 
FiniteFieldElement operator+(const FiniteFieldElement&lhs,const FiniteFieldElement&rhs){
    BigNum add = lhs.i+rhs.i;
    if (add < 0)
        add = rhs.mod_prime + add;
    return FiniteFieldElement(add%rhs.mod_prime);
}
FiniteFieldElement operator-(const FiniteFieldElement&lhs,const FiniteFieldElement&rhs){
    BigNum sub = lhs.i-rhs.i;
    if (sub < 0)
        sub = rhs.mod_prime + sub;
    return FiniteFieldElement(sub%rhs.mod_prime);
}
FiniteFieldElement operator*(const FiniteFieldElement&lhs,const FiniteFieldElement&rhs){
    BigNum mul = lhs.i*rhs.i;
    if (mul < 0)
        mul = rhs.mod_prime + mul;
    return FiniteFieldElement(mul%rhs.mod_prime);
}
FiniteFieldElement operator/(const FiniteFieldElement&lhs,const FiniteFieldElement&rhs){
    BigNum rhs_inv = inv(rhs).i;
    BigNum div = lhs.i*rhs_inv%rhs.mod_prime;
    return FiniteFieldElement(div);
}
FiniteFieldElement operator%(const FiniteFieldElement&lhs,const FiniteFieldElement&rhs){
    BigNum mod = lhs.i%rhs.i;
    if(mod < 0)
        mod = rhs.mod_prime + mod;
    return FiniteFieldElement(mod%rhs.mod_prime);
}
FiniteFieldElement operator-(const FiniteFieldElement&rhs){
    BigNum sub = (rhs.mod_prime - rhs.i)%rhs.mod_prime;
    return FiniteFieldElement(sub);
}

FiniteFieldElement inv(const FiniteFieldElement&rhs){
    vector<BigNum> result = rhs.EEA(rhs.i,rhs.mod_prime);
    BigNum inv = result[0]%rhs.mod_prime;
    if (inv < 0)
        inv = (rhs.mod_prime + inv)%rhs.mod_prime;
    return FiniteFieldElement(inv);
}
// sqrt and power, pow is done with fast pow
vector<FiniteFieldElement> sqrt(FiniteFieldElement& input){
    BigNum p = (input.mod_prime + 1)/4;
    FiniteFieldElement r1 = pow(input, p);
    FiniteFieldElement r2 = (input.mod_prime - r1);
    vector<FiniteFieldElement> result = {r1, r2};
    return result;
}
FiniteFieldElement pow(FiniteFieldElement base, BigNum power){
    if (power == 0)
        return 1;
    else if(power == 1)
        return base;
    FiniteFieldElement prod = pow(base, power / 2);
    prod = prod * prod;
    if(power%2 == 1)
        prod = prod * base;
    return prod;
}
//define the output stream operator output the i value
ostream& operator<<(ostream& os, FiniteFieldElement& rhs){
    os << rhs.i;
    return os;
}
string FiniteFieldElement::to_string() const{
    stringstream ss;
    string s;
    ss<<(*this).i;
    ss>>s;
    return s;
}

