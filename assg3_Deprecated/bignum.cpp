#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "bignum.h"
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
vector<BigNum> BigNum::operator/(BigNum &rhs){
   
    //handles division by zero error 
    if(rhs.digits == 1 && rhs.dt[0] == 0){
        cerr << "Division by 0 error!!" << endl;
        exit(DIVISION_BY_0_ERROR);
    }
    //make comparison for easier implementations
    int k = compare(*this, rhs);
    //if both divisors are the same, return 1  
    if (k == 1){
        cout << "Performing looped version" << endl;
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
        BigNum divisor = BigNum(ss.str());
        vector<BigNum> Result;
        Result.emplace_back(divisor);
        Result.emplace_back(A);
        return Result;
    }
    else if(k == 0){
        vector<BigNum> Result;
        Result.emplace_back(BigNum(1));
        Result.emplace_back(BigNum(0));
        return Result;
    }
    //if B is larger, the result would be 0
    else if(k == -1){
        vector<BigNum> Result;
        Result.emplace_back(BigNum(0));
        Result.emplace_back(rhs);
        return Result;
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
        vector<BigNum> Result = *this/rhs;
        // BigNum Result = A - (fact*rhs);
        return Result[1];
    }
}
vector<BigNum> divide(string s1, string s2){
    int l1 = s1.length(), l2 = s2.length();
    int pos = l2;
    string sub = s1.substr(0,l2); 
    bool padding = false;
    if(s2>sub){
        sub+=s1[pos++]; 
        s2 = "0"+s2;
        padding = true;
    }
    BigNum div1 = BigNum(sub), div2 = BigNum(s2);
    vector<BigNum> result = div1/div2;
    if(padding){
        div2.dt.erase(div2.dt.begin());
        div2.setdigit(div2.getlen()-1);
    }
    BigNum Final = result[0];
    div1 = result[1];
    while (pos < l1){
        cout << "Enter Loop" << endl;
        if(div1.tostring() == "0" && s1[pos] == '0'){
            pos++;
            continue;
        }
        else if(div1.tostring() == "0" && s1[pos]!=0){
            cout << div1.getlen()<< endl;
            sub = s1.substr(pos++,1);
            div1 = BigNum(sub);
        }
        else{
            div1.increasedigit();
            sub = s1.substr(pos++ ,1);
            div1.insertFront(stoi(sub, nullptr, 16));
        }
        if(compare(div1, div2) >= 0){
            result = div1/div2;
            cout << result[0].getlen() << endl;
            Final.appendDt(result[0]); 
            div1 = result[1];
        }
        else
            Final.appendDt(BigNum(0)); 
    }
    result[0] = Final;
    return result;
}
string BigNum::tostring(){
    string lib = "0123456789abcdef";
    string s = "";
    for(auto &i: dt){
        s = lib[i] + s;
    }
    return s;
}