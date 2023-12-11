#include "ECC.hpp"
using namespace std;
//extended euclidean algorithm
ECC::ECC(BigNum& mod_prime, FiniteFieldElement& a, FiniteFieldElement& b, Point& gen, BigNum& order){   
    FiniteFieldElement::mod_prime = mod_prime;
    this->mod_prime = mod_prime;
    this->a = a;
    this->b = b;
    this->gen = gen;
    this->order = order;
}
const vector<Point> ECC::toPoint(const FiniteFieldElement &f){
    FiniteFieldElement total = f * f * f + a * f + b;
    vector<FiniteFieldElement> root = sqrt(total);
    // cout << root[0] << " " << root[1] << endl;
    vector<Point> points = {Point(f,root[0]), Point(f,root[1])};
    return points;
}
// check whether point is on the curve or not
const bool ECC::isOnCurve(const Point& p){
    // y^2 mod prime check if equal x^3 + ax + b mod prime
    return(p.y*p.y) == (p.x*p.x*p.x + a*p.x + b);
}
// check x, y coordinates to determine if point is zero
const bool ECC::isZero(const Point& p){
    return p.x == 0 && p.y == 0;
}
//negate the point
const Point ECC::negate(const Point& p){
    return Point(p.x, -p.y);
}
 //addition is defined as point addition in the sections provided
const Point ECC::addition(const Point& p1, const Point& p2){ 
    Point result;
    FiniteFieldElement s;
    if(p1 == p2)
        return doublePoint(p1);
    if(isZero(p1))
        return p2;
    if(isZero(p2))
        return p1;
    if(p1.x == p2.x&&p1.y==(-1)*p2.y)
        return Point(0,0);
    s = (p2.y-p1.y)/(p2.x-p1.x);
    result.x = s*s-p1.x-p2.x;
    result.y = s*(p1.x-result.x)-p1.y;
    return result;
}
//doubling the point values
const Point ECC::doublePoint(const Point& p){
    Point result;
    FiniteFieldElement s;
    if(isZero(p))
        return p;
    s = (3*p.x*p.x+a)/(2*p.y);
    result.x = s*s-2*p.x;
    result.y = s*(p.x-result.x)-p.y;
    return result;
}
//multiply point P and n 
const Point ECC::multiply(const Point& p,const BigNum& n){
    if(n==1)
        return p;
    else if(n == 2)
        return doublePoint(p);
    Point result = multiply(p, n / 2);
    result = doublePoint(result);
    if(n%2 == 1)
        result = addition(result, p);
    return result;
}
//turn string into FiniteFieldElement and then turn to point
const Point ECC::embed(string& str){
    //apend 00 to mx
    FiniteFieldElement Mx = FiniteFieldElement(str)*256;
    vector<Point> points;
    while(true){
        points = toPoint(Mx);
        if(isOnCurve(points[0])&&(points[0].y % 2 == 1))
            return points[0];
        if(isOnCurve(points[1])&&(points[1].y % 2 == 1))
            return points[1];
        Mx = Mx+1;
    }
}
//basically doing the inverse of the previous op
const string ECC::unembed(Point& p){
    string result = p.x.to_string();
    result.erase(result.end()-2, result.end());
    return result;
}
//decode the string 
const Point ECC::decode(string& str){
    string tmp = str;
    tmp.erase(tmp.begin(), tmp.begin()+2);
    FiniteFieldElement Mx = FiniteFieldElement(tmp);
    vector<Point> points=toPoint(Mx);
    if(points[0].y%2 == 1){
        if(str[1]=='3')
            return Point(Mx,points[0].y);
        else
            return Point(Mx,points[1].y);
    }
    else{
        if(str[1]=='3')
            return Point(Mx,points[1].y);
        else
            return Point(Mx,points[0].y);
    }
}
//encode the point
const string ECC::encode(const Point& p){
    string result = p.x.to_string();
    if(p.y%2 == 1)
        result = "02"+result;
    else
        result = "03"+result;
    return result;
}
//encrypt and decrypt to point 
const vector<Point> ECC::encrypt(Point& p, BigNum& n, Point& p_key){
    vector<Point> result;
    Point Pk = multiply(gen, n);
    Point P_pa_nk = multiply(p_key, n);
    Point Pb = addition(p, P_pa_nk);
    result.emplace_back(Pk);
    result.emplace_back(Pb);
    return result;
}
const Point ECC::decrypt(vector<Point>& c, BigNum& priv_key){
    Point pk = c[0];
    Point pb = c[1];
    Point n_pk = negate(multiply(pk, priv_key));
    Point n_pb = addition(pb, n_pk);
    return n_pb;
}