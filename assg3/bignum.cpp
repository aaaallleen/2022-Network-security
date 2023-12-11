#include<bits/stdc++.h>
#include "bignum.h"
using namespace std;
//four constructors using different input methods;
BigNum::BigNum(){
    sign = true;
    digits = 0;
    data = {};
}
BigNum::BigNum(int a){
    string s;
    stringstream ss;
    ss << hex << a;
    s = ss.str();
    digits = s.length();
    sign = (a>=0)?true:false;
    if (s.length()%2 == 1){
        s = "0"+s;
    }
    for(size_t i = 0; i < s.length()/2; i++){
        string bytestring = s.substr(i*2, 2);
        data.emplace_back(strtoul(bytestring.c_str(), nullptr, 16));
    }
}
BigNum::BigNum(string s){  
    sign = true;
    s.erase(0, s.find_first_not_of('0'));
    if(s[0] == '-'){
        sign = false;
        s = s.substr(1, s.length()-1) ;
    }
    if(s.length() == 0)
        s = "0";
    digits = s.length();
    if (s.length()%2 == 1){
        s = "0"+s;
    } 
    
    for(size_t i = 0; i < s.size(); i+=2){
        string bytestring = s.substr(i, 2);
        data.emplace_back(stoul(bytestring, nullptr, 16));
    }
}
BigNum::BigNum(bool b, int a, vector<uint8_t> v){
    sign = b;
    digits = a;
    data = v;
}
//print the string
void BigNum::print(){
    stringstream ss;
    for(auto &i: data){
        if (i < 0x10 )
            ss << hex << "0";
        ss <<hex << i-0;
    }
    string s = ss.str();
    s.erase(0, s.find_first_not_of('0'));
    if(s.length() == 0)
        s = "0";
    if(!sign)
        s = "-"+s;
    cout << s << endl;
}
//compare two BigNums 
int compare(BigNum a, BigNum b){
    //return 0 for equal, 1 for greater, -1 for less than
    if(a.digits == 1 && a.data[0] == 0 && b.digits == 1 && b.data[0] == 0)
        return 0;
    if(a.sign == true && b.sign == false)
        return 1;
    else if(a.sign == false && b.sign == true)
        return -1;
    else if (a.sign == true && b.sign == true){
        if(a.digits > b.digits)
            return 1;
        else if(b.digits > a.digits)
            return -1;
        else{
            for(int i = 0; i < a.data.size(); i++){
                if(a.data[i] > b.data[i])
                    return 1;
                else if(b.data[i] > a.data[i])
                    return -1;
            }
        }
        return 0;
    }
    else if (a.sign == false && b.sign == false){
        if(a.digits > b.digits)
            return -1;
        else if(b.digits > a.digits)
            return 1;
        else{
            for(int i = 0; i < a.data.size(); i++){
                if(a.data[i] > b.data[i])
                    return -1;
                else if(b.data[i] > a.data[i])
                    return 1;
            }
        }
        return 0;
    }
    return 0;
}
//clean all the leading 0s 
string makeclean(vector<uint8_t> v){
    reverse(v.begin(), v.end());
    stringstream ss;
    for(const auto &i: v){
        if(i < 0x10)
            ss << hex << "0";
        ss <<hex << i-0;
    }
    string s = ss.str();
    s.erase(0, s.find_first_not_of('0'));
    if(s.length() == 0 )
        s = "0";
    return s;
}
//BigNum + overload operator
BigNum BigNum::operator+(BigNum& rhs){
    //for A+B where A is neg, B is pos, we turn it into B-A. this way
    if(sign == false && rhs.sign == true){
        setsign(true);
        BigNum res = rhs-*this;
        setsign(false);
        return res;
    } 
    //for A+B where A is pos, B is neg, turn it into A-B.
    else if(sign == true && rhs.sign == false){
        rhs.setsign(true);
        BigNum res = *this-rhs;
        rhs.setsign(true);
        return res;
    }
    //perform normal addition when encountering both pos
    else if(sign == true && rhs.sign == true){
        int ld = ceil((double)digits/2), rd = ceil((double)rhs.digits/2), diff = abs(rd-ld);
        int flag = (digits>rhs.digits)?1:0;
        vector<uint8_t> l= data, r =rhs.data;
        reverse(l.begin(), l.end());
        reverse(r.begin(), r.end());
        uint8_t carry = 0x00;
        vector<uint8_t> sum={};
        for(int i = 0; i < min(rd, ld); i++){
            int left = l[i], right = r[i], carryint = carry;
            int result = (left+right+carry)%256;
            carry = (result<left || result < right)?0x01:0x00;
            sum.emplace_back(static_cast<uint8_t>(result));
        } 
        for(int i = min(rd, ld); i < min(rd, ld)+diff; i++){
            uint8_t result = (flag)?l[i]+carry:r[i]+carry;
            carry = 0x00;
            sum.emplace_back(result);
        }
        if(carry>0x00)
            sum.emplace_back(carry);
        string s = makeclean(sum);
        return BigNum(s);
    }
    //if both are neg, we turn both into pos and return -(A+B)
    else if(sign == false && rhs.sign == false){
       setsign(true);
       rhs.setsign(true);
       BigNum result = *this+rhs;
       result.setsign(false);
       setsign(false);
       rhs.setsign(false);
       return result;
    }
}
BigNum BigNum::operator-(BigNum& rhs){
    //if both are positive, do normal sub
    if(sign == true&&rhs.sign == true){
        int compresult = compare(*this, rhs);
        if(compresult == 0)
            return BigNum(0);
        // if A-B where B > A, we return - (B-A)
        else if (compresult == -1){
            BigNum result = rhs-*this;
            return BigNum(!result.sign, result.digits, result.data);
        }    
        else{
            int ld = ceil((double)digits/2), rd = ceil((double)rhs.digits/2), diff = abs(rd-ld);
            vector<uint8_t> ldata = data, rdata = rhs.data;
            reverse(ldata.begin(), ldata.end());
            reverse(rdata.begin(), rdata.end());
            int borrow = 0;
            vector<uint8_t> div;
            for(int i =0; i < rd; i++){
                int left = ldata[i];
                int right = rdata[i];
                int result = left - right - borrow;
                borrow = 0;
                if(result < 0){
                    borrow=1;
                    result+= 256;
                }
                div.emplace_back(static_cast<uint8_t>(result));
            }
            for(int i= rd; i < ld; i++){
                int left = ldata[i];
                int result = left - borrow;
                borrow = 0;
                if(result < 0){
                    borrow=1;
                    result+= 256;
                }
                div.emplace_back(static_cast<uint8_t>(result));
            }
            string s = makeclean(div);
            return BigNum(s);
        }
    }
    // if A-B and B is neg, we return A+(-B)
    else if(sign == true && rhs.sign == false){
        BigNum posrhs = BigNum(true, rhs.digits, rhs.data);
        return *this+posrhs;
    }
    // if A-B and A is neg, return - (A+B)
    else if(sign == false && rhs.sign == true){
        setsign(true);
        BigNum result = *this+rhs;
        result.setsign(false);
        setsign(false);
        return result;
    }
    //else if both are neg, have to determine which one is bigger in order to determine result is pos/neg 
    else if(sign == false && rhs.sign == false){
        setsign(true);
        rhs.setsign(true);
        int k = compare(*this, rhs);
        bool posneg = (k <= 0)?true:false;
        BigNum res = rhs-*this;
        setsign(false);
        rhs.setsign(false);
        return BigNum(posneg, res.digits, res.data);
    }
}
BigNum BigNum::operator*(BigNum& rhs){
    if((digits == 1 && data[0]==0x00 )|| (rhs.digits==1 && rhs.data[0] == 0x00)){
        return BigNum(0);
    }
    int ld = ceil((double)digits/2), rd = ceil((double)rhs.digits/2), diff = abs(rd-ld);
    int resdigit = ld+rd;
    //set the size of the vector
    vector<int> mulres;
    mulres.resize(resdigit);
    vector<uint8_t> ldata = data, rdata = rhs.data;
    reverse(ldata.begin(), ldata.end());
    reverse(rdata.begin(), rdata.end());
    //implement the direct approach of multiplication
    for(int i = 0; i < ld; i++){
        for (int j = 0; j <rd; j++){
            int data1 = ldata[i], data2 = rdata[j];
            int prod = data1*data2;
            mulres[i+j] += prod%256;
            mulres[i+j+1] += prod/256;
        }
    }
    for(int i = 0; i < resdigit; i++){
        mulres[i+1] += mulres[i]/256;
        mulres[i] %=256; 
    }
    vector<uint8_t> result;
    for(size_t i = 0 ; i < mulres.size(); i++){
        result.emplace_back(static_cast<uint8_t>(mulres[i]));
    }
    string s = makeclean(result);
    BigNum final = BigNum(s);
    //the sign is negation of xor, since pos pos => pos, neg neg => pos, neg pos => neg, pos neg => neg
    bool signs = !(sign^rhs.sign);
    return  BigNum(signs, final.digits, final.data);
}
string BigNum::tostring(){
    stringstream ss;
    for(auto &i : data){
        if(i < 0x10)
            ss << hex << "0";
        ss <<hex << i-0;
    }
    string s = ss.str();
    s.erase(0, s.find_first_not_of('0'));
    if(s.length() == 0)
        s = "0";
    return s;
}
//transform int array to BigNumber, used in *
BigNum intarrtoBN(vector<int> v){
    stringstream ss;
    for(size_t i = 0; i < v.size(); i++){
        if(v[i] < 10)
            ss << v[i];
        switch (v[i]){
        case 64:
            ss << "0";
            break;
        case 10:
            ss << "a";
            break;
        case 11:
            ss << "b";
            break;
        case 12:
            ss << "c";
            break;
        case 13:
            ss << "d";
            break;
        case 14:
            ss << "e";
            break;
        case 15:
            ss << "f";
            break;
        default:
            break;
        }
    }
    string s = ss.str();
    s.erase(0, s.find_first_not_of('0'));
    if(s.length() == 0)
        s = "0";
    return BigNum(s);
}
//do modular exponential to improve speed;
BigNum modexp(BigNum a, BigNum power, BigNum mod){
    if(compare(power, BigNum(1)) == 0){
        vector<BigNum> result = a/mod;
        return result[1];
    }  
    else if(compare(power, BigNum(0)) == 0)
        return BigNum(1);
    BigNum two = BigNum(2);
    // a^2k can be done as (a^2)^k
    vector<BigNum> time2 = power/two;
    BigNum powdiv2 = time2[0];
    BigNum product = modexp(a, powdiv2, mod);
    product = product*product;
    vector<BigNum> nprod = product/mod;
    product = nprod[1];
    vector<uint8_t> dt = power.getdata();
    int n = dt.size();
    // if the power is odd, we have to multiply it with base again
    int last = dt[n-1]%2;
    if(last == 1){
        product=product*a;
        nprod = product/mod;
        product = nprod[1];
    }
    return product; 
}
//similar to other operators, we transform the results if dividen or divisor is negative
vector<BigNum> BigNum::operator/(BigNum& rhs){
    if (rhs.digits == 1 && rhs.data[0] == 0x00){
        cerr << "Division by 0 error" << endl;
        exit(DIVISION_BY_0_ERROR);
    }
    if(sign == true && rhs.sign == true){
        int comp = compare(*this, rhs);
        if(comp == 0)
            return {BigNum(1), BigNum(0)};
        else if( comp == -1)
            return {BigNum(0), *this};
        else if( comp == 1){
            string lstr = this->tostring(), rstr = rhs.tostring(),divstr;
            int ldigit = lstr.length(), rdigit = rstr.length(), pos = 0;
            divstr = lstr.substr(pos, rdigit);
            pos = rdigit;
            BigNum divisor =rhs;
            vector<int> quotient;
            int quo = 0;
            BigNum dividend;
            int cnt = 0;
            while(pos <= ldigit){
                // cout << "round " << ++cnt ;
                dividend = BigNum(divstr);
                if(compare(dividend, divisor) == -1){
                    // cout << " smaller" << endl;
                    divstr+=lstr[pos++];
                    quotient.emplace_back(0);
                    continue;
                }
                while(compare(dividend, divisor) != -1){
                    dividend = dividend-divisor;
                    quo++;
                }
                quotient.push_back(quo);
                quo = 0;
                divstr = dividend.tostring();
                divstr+=lstr[pos++];
                // cout << endl;
                // dividend.print();
            }
            reverse(dividend.data.begin(), dividend.data.end());
            string s = makeclean(dividend.data);
            return {intarrtoBN(quotient), BigNum(s)};
        }
    }
    else if (sign == true && rhs.sign == false){
        rhs.setsign(true);
        vector<BigNum> result = *this/rhs;
        result[0].setsign(false);
        rhs.setsign(false);
        return {result[0], result[1]};
    }
    else if (sign == false && rhs.sign == true){
        setsign(true);
        vector<BigNum> result = *this/rhs;
        result[0].setsign(false);
        BigNum rem = rhs-result[1];
        setsign(false);
        return {result[0], rem};
    }
    else if (sign == false && rhs.sign == false){
       setsign(true);
       rhs.setsign(true);
       vector<BigNum> result = *this/rhs;
       result[1] = rhs - result[1];
       setsign(false);
       rhs.setsign(false);
       return {result[0], result[1]};
    }
}
//do extended euclidean algorithm for BigNum P and Q
BigNum EEA(BigNum p, BigNum q, BigNum &a, BigNum& b){
    BigNum zero = BigNum(0);
    BigNum one = BigNum(1);
    if(compare(zero, p) == 0){
        a = zero;
        b = one;
        return q;
    }
    BigNum a1, b1;
    vector<BigNum> divres = q/p; 
    BigNum div = divres[0];
    BigNum rem = divres[1];
    BigNum gcd = EEA(rem, p, a1, b1);
    // gcd.print();
    BigNum n = div*a1;
    //divres[0].print();
    a = b1-n;
    b = a1;
    return gcd;
}
