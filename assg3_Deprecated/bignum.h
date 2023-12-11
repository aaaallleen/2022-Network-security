#ifndef NS_ASSG1_H
#define NS_ASSG1_H
#define DIVISION_BY_0_ERROR 1
#define BIGNUM_CONSTRUCTOR_ERROR 2
#define COMPARE_ERROR 3
#define DIVISION_ERROR 4
#include <vector>
#include <string>

class BigNum{
    private:
        //add sgn to the BigNum true for positive, false for negative
        bool sgn; 
        //digits for easy comparison
        int digits;
        //transform the hex into integers for easy calculation
        std::vector<int> dt;
    public:
        //define 3 constructors, the BigNum(int) is only used for constructing basic values such as 0 and 1
        BigNum();
        BigNum(int);
        BigNum(std::string);

        //define the basic operands
        BigNum operator+(BigNum&);
        BigNum operator-(BigNum&);
        BigNum operator*(BigNum&);
        std::vector<BigNum> operator/(BigNum&);
        BigNum operator%(BigNum&);
        //some functions to set/get the private values, defined just in case the project is extended 
        void Print();
        int getlen(){
            return digits;
        };
        bool checksign(){
            return sgn;
        }
        void setsign(bool b){
            sgn = b;
        }
        void setdigit(int n){
            digits = n;
        }
        void increasedigit(){
            digits++;
        }
        void appendDt(int a){
            dt.push_back(a);
        }
        void insertFront(int a){
            dt.insert(dt.begin(), a);
        }
        
        void appendDt(BigNum a){
            this->digits += a.digits;
            this->dt.insert(this->dt.begin(), a.dt.begin(), a.dt.end());
        }
        std::vector<int> getdt(){
            return dt;
        }
        std::string tostring();
        //since values have different lengths, function to pad zero to simplify the calculations
        void PadZero(BigNum&);
        friend std::vector<BigNum> divide(std::string , std::string );
         
};
//compare two bignums 
int compare(BigNum&, BigNum&);

#endif