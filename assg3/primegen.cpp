#include<bits/stdc++.h>
#include "primegen.h"
#include "bignum.cpp"
using namespace std;
//test the small primes for accelerating the proccess;
bool smallprimetest(BigNum N){
    BigNum zero = BigNum(0);
    for (auto &i: smallprime){
        BigNum sp = BigNum(i);
        vector<BigNum> result = N/sp;
        if( compare(result[1], zero) == 0){
            return false;
        }
            
    }
    return true;
}
//generate the string for the possible prime
vector<string> genbignumstr(){
    string bignumstr = "", bignumstr_1 = ""; 
    for(int i = 0; i < 64; ++i){
        int n = rand();
        if(i == 0){
            //the first bit cannot be 0;
            bignumstr+= values[n%15+1];
            bignumstr_1 += values[n%15+1];
        }   
        else if(i == 63)
        {
            //in order to be prime, the last digit must be odd, and adjust N-1 also
            bignumstr += values[n%8 *2 +1];
            bignumstr_1 += values[n%8 *2];
        }     
        else {
            bignumstr += values[n%16];
            bignumstr_1+= values[n%16];
        }
            
    }
    return {bignumstr, bignumstr_1};
}
//find the k so that N-1 = 2^k*m
int findK(BigNum N1){
    vector<uint8_t> dt = N1.getdata();
    int k = 0;
    reverse(dt.begin(), dt.end());
    //reverse the data array for easier checking
    uint8_t mask = 0x01;
    // use bitwise and and the mask to check if last digit is 1, if it is, we found k since the number cannot be divided by 2 anymore
    for(auto &i: dt){
        //if the 8 bits are 00000000, we can skip the 8 bits
        if (i == 0x00)
            k+=8;
        else{
            //otherwise we check bit by bit using bitwise and and rightshift
            uint8_t res = i, cur = i;
            for(int j = 0; j <8; j++){
                res = cur & mask;
                if(res == 0x01){
                    return k;
                }
                else{
                    cur >>= 1;
                    ++k;
                }
            }
        }
    }
    return k;
}
BigNum genBigPrime(){ 
    vector<string> str = genbignumstr();
    string bignumstr = str[0], bignumstr_1 = str[1];
    int tries = 0;
    bool isprime = false;
    while(!isprime){ //if it is not prime, we iterate over and over again, this could take quite a long time
        BigNum N = BigNum(bignumstr);
        BigNum N_1 = BigNum(bignumstr_1);
        //do small prime test to increase speed;
        if(!smallprimetest(N)){
            isprime = false;
            str = genbignumstr();
            bignumstr = str[0], bignumstr_1 = str[1];
            continue;
        }
        int k = findK(N_1);
        BigNum zero = BigNum(0);
        BigNum one = BigNum(1);
        BigNum two = BigNum(2);
        BigNum powtwo = modexp(two, BigNum(k), N);
        vector<BigNum> result = N_1/powtwo;
        //find the m since m = N-1/2^k
        BigNum m = result[0];
        int flag, cnt = 0;
        for(int i = 0; i < 16; i++){ 
            //the Miller-Rabin test
            isprime = true;
            flag = 0;
            string s = genbignumstr()[0];
            s = s.substr(0, 63);
            BigNum a = BigNum(s);
            BigNum y = modexp(a, m, N);
            if(compare(y, one)!= 0 && compare(y, N_1)!=0){
                int j = 1;
                while( j <= k-1 && compare(y, N_1)!=0){
                    y = ((y*y)/N)[1];
                    if(compare(y , one) == 0){
                        flag = 1;
                        break;
                    }
                    ++j;
                }
                if(flag == 1){
                    isprime = false;
                    break;
                }
                if(compare(y, N_1) !=0 ){
                    isprime = false;
                    break;
                }
            }
        }
        if(isprime == true){
            return N;
        }
        else{
            str = genbignumstr();
            bignumstr = str[0], bignumstr_1 = str[1];
        }
    }
    return BigNum(bignumstr);

}
