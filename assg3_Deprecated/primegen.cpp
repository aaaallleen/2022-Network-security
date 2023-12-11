#include<bits/stdc++.h>
#include "primegen.h"
#include "bignum.cpp"
using namespace std;
vector<int> findK_M(string s1){
    int k = 0;
    vector<BigNum> result;
    result = divide(s1, "2");
    result[0].Print();
    BigNum m = result[0];
    while(result[1].tostring() == "0" ){
        //result[0].Print();
        m=result[0];
        string s = result[0].tostring();
        result = divide(s, "2");
        result[0].Print();
        ++k;
    }
    cout << k << endl;
    m.Print();
    return {1,1};
}
int main(){
    string bignumstr = "";
    string bignumstr_1 = "";
    srand(1);
    for(int i = 0; i < 64; ++i){
        if(i == 0)
            bignumstr+= values[rand()%15+1];
        else if(i == 63)
            bignumstr += values[rand()%8 *2 +1];
        else 
            bignumstr += values[rand()%16];
    }
    cout << bignumstr<< endl;
    bignumstr_1 = bignumstr;
    bignumstr_1[63] = bignumstr_1[63]-1;
    //string num = "230";
    vector<int> km = findK_M(bignumstr_1);
   
}