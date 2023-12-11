#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "assg1.h"
using namespace std;
//basic constructor
BigNum::BigNum(){
    sgn = true;
    digits = 0;
}
//this only handles single digit BigNums eg. BigNum(0), BigNum(1);
BigNum::BigNum(int a){
   
    if(a >=10 ){
        cerr<<"BigNum(int) handle error, this constructor is only used for easy construction of 0 or 1s";
        exit(BIGNUM_CONSTRUCTOR_ERROR);
    }
    sgn = true;
    digits = 1;
    dt.push_back(a);
}
//handles the input string
BigNum::BigNum(std::string str){
    //assume all inputs are positive
    sgn = true;
    //obtain the digits since it makes the computation easier?
    digits = str.length();
    //reverse strings so that the LSB is at the front
    reverse(str.begin(), str.end());
    for(int i = 0; i < digits; i++){
        string sub = str.substr(i,1);
        int d = stoi(sub, nullptr, 16);
        dt.push_back(d);
    }
}

//comparator is used in division, subtraction, etc.
int compare(BigNum& A, BigNum& B){
    //return 1 if A > b, -1 if A < B, 0 if A == B 
    int A_len = A.getlen();
    int B_len = B.getlen();
    //use the length of the string to make easy comparison
    if(B_len > A_len){
        return -1;
    }
    else if(A_len > B_len){
        return 1;
    }
    else{
        //if length is the same, since we reverse the vector when constructing, we have to reverse the vector again so that the MSB is at front
        std::vector<int> tA = A.getdt();
        std::vector<int> tB = B.getdt();
        reverse(tA.begin(), tA.end());
        reverse(tB.begin(), tB.end());
        for(int i = 0; i < A_len; i++){
            
            if(tA[i] - tB[i] > 0){
                return 1;
            }   
            else if(tA[i] - tB[i]<0) {
                return -1; 
            }
        }
        
    }
    //after all comparison if still haven't return, the two strings are the same.
    return 0;
}

//to make calculations easiers
void BigNum::PadZero(BigNum& B){
    int A_len = getlen();
    int B_len = B.getlen();
    int diff = A_len-B_len;
    if(diff > 0){
        //A is longer than B, do padding to B;
        for(int i = 0; i<diff; i++)
            B.dt.push_back(0);
    }
    else if(diff < 0){
        //else if B is longer, do padding to A
        diff*= -1;
        for(int i = 0; i<diff; i++)
            dt.push_back(0);
    }
    else{
        return;
    }
}
//print function, use stringstream for easy implementation
void BigNum::Print(){
    
    stringstream ss;
    if(!sgn) cout<<"-";
    for(int i = 0; i < digits; i++){
        ss << hex << dt[i];
    }
    string s = ss.str();
    reverse(s.begin(),s.end());
    cout << s << endl;
    
}
//overload the + operator
BigNum BigNum::operator+( BigNum &rhs){
    BigNum Result = BigNum();
    //do the direct type addition, we already reversed the vector so the LSB is in front
    int carry = 0;
    int len = max(digits, rhs.digits);
    for (int i = 0; i < len; i++){
        int sum = carry + dt[i] + rhs.dt[i];
        carry = sum /16;
        Result.increasedigit();
        Result.appendDt(sum%16);
    }
    if(carry){
        Result.increasedigit();
        Result.appendDt(carry);
    }
    return Result;
}

//overload the operator - 
BigNum BigNum::operator-(BigNum &rhs){
    //do a comparison, if both numbers are the same, return BigNum(0)
    int k = compare(*this, rhs);
    
    if( k == 0){
        return BigNum(0);
    }
    else if( k == 1){
        // A is bigger than B
        BigNum Result = BigNum();
        int borrow = 0;
        int n = digits;
        int k = 0;
        for(int i = 0; i < n ; i++){
            int sub = dt[i] - borrow - rhs.dt[i];
            if(sub<0){
                sub+=16;
                borrow = 1;
            }
            else{
                borrow = 0;
            }
            Result.appendDt(sub);
            Result.increasedigit();
        }
        //remove the 0s in front
        while(Result.dt.size()>=1 && Result.dt.back()==0){
            Result.dt.pop_back();
            Result.setdigit(Result.getlen()-1);
        }
        return Result;
    }
    else if(k == -1){
        // B is bigger than A
        BigNum Result = BigNum();
        Result.setsign(false);
        int borrow = 0;
        int n = rhs.digits;
        for(int i = 0; i < n; i++){
            int sub = rhs.dt[i] - borrow - dt[i];
            if(sub<0){
                sub+=16;
                borrow = 1;
            }
            else{
                borrow = 0;
            }
            Result.appendDt(sub);
            Result.increasedigit();
        }
        //remove the 0s in front
        while(Result.dt.size()>=1 && Result.dt.back()==0){
            Result.dt.pop_back();
            Result.setdigit(Result.getlen()-1);
        }
        return Result;
    }
    else{
        cerr << "Error when comparing"<< endl;
        exit(COMPARE_ERROR);
    }
}
//overload the * operator
BigNum BigNum::operator*(BigNum &rhs){
    //if one of the two multipliers are 0, return 0;
    if((digits == 1 && dt[0]==0 )|| (rhs.digits==1 && rhs.dt[0] == 0)){
        return BigNum(0);
    }
    BigNum Result = BigNum();
    //suppose the result will be the length of sum of the two multipliers
    Result.setdigit(digits + rhs.digits);
    //set the size of the vector
    Result.dt.resize(Result.digits);
    //implement the direct approach of multiplication
    for(int i = 0; i < digits; i++){
        for (int j = 0; j < rhs.digits; j++){
            int prod = dt[i]*rhs.dt[j];
            Result.dt[i+j] += prod%16;
            Result.dt[i+j+1] += prod/16;
        }
    }
    //re-check the vectors since addition might cause the values to go over 16
    for(int i = 0; i < Result.digits; i++){
        Result.dt[i+1] += Result.dt[i]/16;
        Result.dt[i] %=16; 
    }
    //remove the 0s in front
    while(Result.dt.size()>=1 && Result.dt.back()==0){
        Result.dt.pop_back();
        Result.setdigit(Result.getlen()-1);
    }
    return Result;
}
//the / operator
BigNum BigNum::operator/(BigNum &rhs){
   
    //handles division by zero error 
    if(rhs.digits == 1 && rhs.dt[0] == 0){
        cerr << "Division by 0 error!!" << endl;
        exit(DIVISION_BY_0_ERROR);
    }
    //make comparison for easier implementations
    int k = compare(*this, rhs);
    //if both divisors are the same, return 1
    if(k == 0){
        return BigNum(1);
    }
    //if B is larger, the result would be 0
    else if(k == -1){
        return BigNum(0);
    }
    else if (k == 1){
        BigNum A = *this;
        BigNum B = rhs;
        long long int count = 0;
        while(compare(A, B) >=0){
            int n = A.digits;
            A= A-rhs;
            //if the subtraction causes the lhs operator digit to decrease, since we already padded zeros for the RHS operator, we have to adjust accordingly, 
            while(B.dt.back() == 0 && n > 0){
                B.dt.pop_back();
                
            }
            count++;
        }
        //set output
        stringstream ss;
        ss<<hex<<count;
        BigNum Result = BigNum(ss.str());
        return Result;;
    }
    else{
        cerr << "DIVISION ERROR" << endl;
        exit(DIVISION_ERROR);
    }
}
//just handles errors, the implementation is used with the implementation of other operators
BigNum BigNum::operator%(BigNum &rhs){
    if(rhs.digits == 1 && rhs.dt[0] == 0){
        cerr << "Division by 0 error!!" << endl;
        exit(DIVISION_BY_0_ERROR);
    }
    int k = compare(*this, rhs);
    if(k == 0){
        return BigNum(0);
    }
    else if(k == -1){
        return rhs;
    }
    else{
        BigNum A = *this;
        BigNum fact = A/rhs;
        BigNum prod = fact*rhs;
        BigNum Result = A - prod;
        // BigNum Result = A - (fact*rhs);
        return Result;
    }
}
int main(){
    string a,b;
    cout << "a= ";
    cin >> a;
    cout << "b= ";
    cin >> b;
    BigNum A = BigNum(a);
    BigNum B = BigNum(b);
    A.PadZero(B);
    BigNum sum = A+B;
    BigNum sub = A-B;
    BigNum mul = A*B;
    BigNum div = A/B;
    BigNum rem = A%B;

    cout <<endl<< "a + b = ";
    sum.Print();
    cout << endl;

    cout << "a - b = ";
    sub.Print();
    cout << endl;
    
    cout << "a * b = ";
    mul.Print();
    cout << endl;

    cout << "a / b = ";
    div.Print();
    cout << endl;

    cout << "a % b = ";
    rem.Print();
    cout << endl;

}