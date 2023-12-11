#ifndef BIG_NUM_H
#define BIG_NUM_H
#define DIVISION_BY_0_ERROR 1
#define BIGNUM_CONSTRUCTOR_ERROR 2
#define COMPARE_ERROR 3
#define DIVISION_ERROR 4
#include <vector>
#include <string>
class BigNum{
    private:
    //sign true for positive, false for negative;
        bool sign;
        unsigned int digits;
        std::vector<uint8_t> data;
    public:
        BigNum();
        BigNum(int);
        BigNum(std::string);
        BigNum(bool, int, std::vector<uint8_t>);
        BigNum operator+(BigNum&);
        BigNum operator-(BigNum&);
        BigNum operator*(BigNum&);
        std::vector<BigNum> operator/(BigNum&);
        std::string tostring();
        void print();
        int getdigits(){ return digits;}
        bool getsign() {return sign;}
        bool setsign(bool b) {sign=b; }
        std::vector<uint8_t> getdata() { return data;}
        friend int compare(BigNum, BigNum);
};
BigNum intarrtoBN(std::vector<int>);
BigNum modexp(BigNum, BigNum, BigNum);
BigNum EEA(BigNum, BigNum, BigNum&, BigNum &);
std::string makeclean(std::vector<uint8_t>);
#endif