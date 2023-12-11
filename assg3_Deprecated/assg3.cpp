#include<bits/stdc++.h>
#include "bignum.cpp"
using namespace std;
int main(){
    cout << "<Miller-Rabin>" << endl;
    cout << "<Rabin Encryption>" << endl;
    string p="daaefe652cad1614f17e87f2cd80973f",q="f99988626723eef2a54ed484dfa735c7",s="", plaintext="be000badbebadbadbad00debdeadfacedeafbeefadd00addbed00bed0bed";;
    // cout << "p = ";
    // for(int i = 0; i < 4; i++){
    //     cin >> s;
    //     p+=s;
    // }
    // cout << "q = ";
    // for(int i = 0; i <4; i++){
    //     cin >> s;
    //     q+=s;
    // }
    // BigNum P = BigNum(p);
    // BigNum Q = BigNum(q);
    // BigNum N = P*Q;
    // cout << "n = p*q = ";
    // cout << N.tostring() << endl; 
    
    // cout << "Plaintext = ";
    // for(int i = 0; i < 7; i++){
    //     cin >> s;
    //     plaintext+=s;
    // }
    // plaintext+= plaintext.substr(plaintext.size()-4);
    // cout << plaintext << endl;
    // BigNum ptxt = BigNum(plaintext);
    // BigNum Ctxt = ptxt * ptxt;
    // cout << "P = " << P.tostring()<< endl;
    // cout << "Q = " << Q.tostring()<< endl;
    // cout << "N = " << N.tostring()<< endl;
    // cout << "plaintext*plaintext= " << Ctxt.tostring() << endl;
    // vector<BigNum> encrypt = divide(Ctxt.tostring(),  N.tostring());
    // Ctxt = encrypt[1];
    // Ctxt.Print();
    vector<BigNum> td = divide("1c", "2");
    td[0].Print();
    td[1].Print();
}