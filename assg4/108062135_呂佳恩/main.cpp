#include "BigNum.cpp"
#include "Point.cpp"
#include "FiniteFieldElement.cpp"
#include "ECC.cpp"
#include <bits/stdc++.h>
using namespace std;
//use to pad spaces and zeros in the output, the flag is for determining whether to pad zeros or not.
string padspace(FiniteFieldElement x, bool pad){
    string tmp = x.to_string();
    int n = tmp.length()%8;
    if(n!= 0 && pad){
        for(int i = 0; i < 8 - n; i++)
            tmp = "0" + tmp;	
    }
    string result = "";
    for(auto i = 0; i < tmp.length(); i++){
        result += tmp[i];
        if(i % 8 == 7 && i != tmp.length() - 1)
            result += " ";
    }
    
    return result;
}
int main (){
    BigNum p = BigNum("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF");
    BigNum n =  BigNum("0100000000000000000001F4C8F927AED3CA752257");
    FiniteFieldElement::mod_prime = p;
    FiniteFieldElement a =  FiniteFieldElement("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC");
    FiniteFieldElement b =  FiniteFieldElement("1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45");
    FiniteFieldElement Gx = FiniteFieldElement("4A96B5688EF573284664698968C38BB913CBFC82");
    FiniteFieldElement Gy = FiniteFieldElement("23A628553168947D59DCC912042351377AC5FB32");
    Point g = Point(Gx,Gy);
    ECC ec = ECC(p,a,b,g,n);
    cout << "<EC-ElGamal encryption>" << endl;
    string m, pa, nk, tmp;
    cout << "Plaintext M = ";
    while(m.length() < 38){
        cin >> tmp ;
        m += tmp;
    }
    cout << "Pa = ";
    while(pa.length() < 42){
        cin >> tmp;
        pa += tmp;
    }
    cout << "nk = ";
    while(nk.length() < 40){
        cin >> tmp;
        nk += tmp;
    }
    BigNum NK = BigNum(nk);
    Point Pm = ec.embed(m);
    cout << "Mx = " << padspace(Pm.x, true) << endl;
    cout << "My = " << padspace(Pm.y, true)<< endl;	 
    //public key
    Point Pa = ec.decode(pa);
    vector<Point> Cm = ec.encrypt(Pm, NK, Pa);
    cout << "Cm = {Pk, Pb} = {" << padspace(Cm[0].x, true) << ", " << padspace(Cm[1].x, true) << "}" << endl;
    cout << endl;
    cout << "<EC-ElGamal decryption>" << endl;
    string dec_pk, dec_pb, dec_na;
    cout << "Pk = ";
    while(dec_pk.length() < 42){
        cin >> tmp;
        dec_pk += tmp;
    }
    cout << "Pb = ";
    while(dec_pb.length() < 42){
        cin >> tmp;
        dec_pb += tmp;
    }
    cout << "na = ";
    while(dec_na.length() < 40){
        cin >> tmp;
        dec_na += tmp;
    }
    BigNum NA = BigNum(dec_na);
    
    Point dec_pk_pnt = ec.decode(dec_pk);
    Point dec_pb_pnt = ec.decode(dec_pb);
    vector<Point> dec_pnts = {dec_pk_pnt, dec_pb_pnt};
    Point dec_pnt = ec.decrypt(dec_pnts, NA);
    FiniteFieldElement m_finish = ec.unembed(dec_pnt);
    cout << "Plaintext = " << padspace(m_finish, false) << endl;
    return 0;
}