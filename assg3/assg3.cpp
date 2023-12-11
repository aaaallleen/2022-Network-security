#include <bits/stdc++.h>
#include "primegen.cpp"
#include "assg3.h"
using namespace std;
//prints the string with a space every 8 bits
void printstring(string s){
    for(size_t i = 0; i < s.length(); i++){
        cout << s[i];
        if(i%8==7 && i != 0)
            cout << " ";
    }
    cout << endl;
}
//Encrypt the string with p and q
string RabinEncyption(){
    string p, s, q,  plaintext;
    cout<< "p = ";
    for(int i = 0; i < 4; i++){
        cin >> s ;
        p+=s;
    }
    cout<< "q = ";
    for(int i = 0; i < 4; i++){
        cin >> s;
        q+=s;
    }
    BigNum P = BigNum(p);
    BigNum Q = BigNum(q);
    BigNum N = P*Q;
    cout << "n = pq = ";
    printstring(N.tostring());
    cout << "Plaintext: ";
    for(int i = 0; i < 7; i++){
        cin >> s;
        plaintext+= s;
    }
    //do padding for the last 4 bits
    plaintext += plaintext.substr(plaintext.length()-4, 4);
    BigNum ptx = BigNum(plaintext);
    ptx = ptx*ptx;
    vector<BigNum> ciphertext = ptx/N;
    //return the remainder as the ciphertext
    return ciphertext[1].tostring();
}
// handles the case of 3 mod 4
BigNum sqrt_34(BigNum a, BigNum num){
    BigNum four = BigNum(4);
    BigNum one = BigNum(1);
    BigNum zero = BigNum(0);
    BigNum pow = num+one;
    vector<BigNum> times = pow/four;
    if(compare(times[1], zero) != 0){
        cout << "div error"<< endl;
        cout << times[1].getsign() << "\t" << times[1].getdigits() << endl;
        times[1].print();
    }
    pow = times[0];
    BigNum r = modexp(a, pow, num);
    times = r/num;
    return times[1];
}
//if the case is 5 mod 8
BigNum sqrt_58(BigNum a, BigNum num){
    BigNum four = BigNum(4);
    BigNum one = BigNum(1);
    BigNum zero = BigNum(0);
    BigNum eight = BigNum(8);
    //calculate d = a^(p-1)/4 
    BigNum pow = num-one;
    vector<BigNum> times = pow/four;
    pow = times[0];
    BigNum r = modexp(a, pow, num);
    //get the remainder mod p
    times = r/num;
    BigNum d = times[1];
    //for two cases, d = 1 or -1 mod p
    if(compare(d, one) == 0){
        //r = a^(p+3)/8 mod p 
        BigNum three = BigNum(3);
        pow = num + three;
        times = pow/eight;
        pow = times[1];
        r = modexp(a, pow, num);
        times = r/num;
        return times[1];
    }
    else{
        //r = 2a*(4a)^(p-5)/8 mod p 
        BigNum two = BigNum(2);
        BigNum five = BigNum(5);
        BigNum twoA = two*a;
        BigNum fourA = four*a;
        pow = num-five;
        times = pow/eight;
        pow = times[0];
        r = modexp(fourA, pow, num);
        r = r*twoA;
        times = r/num;
        return times[1];
    }
}
string RabinDecryption(){
    //IO handling
    string p, q, ciphertext, s;
    cout << "Ciphertext = ";
    for(int i = 0; i < 8; i++){
        cin >> s;
        ciphertext += s;
    }
    cout << "Private Key:" << endl;
    cout << "p = ";
    for(int i = 0; i < 4; i++){
        cin >> s;
        p+=s;
    }
    cout <<"q = ";
    for(int i = 0; i < 4; i++){
        cin >> s; 
        q+=s;
    }
    BigNum a, b, P = BigNum(p), Q = BigNum(q), C = BigNum(ciphertext) ;
    BigNum PQ = P*Q;
    vector<BigNum> C_modpq = C/PQ;
    C = C_modpq[1];
    int plast = stoi(string(1,p[p.length()-1]), nullptr, 16);
    int qlast = stoi(string(1,q[p.length()-1]), nullptr, 16);
    BigNum sqrtpow_p, sqrtpow_q;\
    //check the case if it is 3mod 4 or 5 mod 8
    if(plast%4 == 3){
        sqrtpow_p = sqrt_34(C, P);
    }       
    else if (plast%8 == 5)
        sqrtpow_p = sqrt_58(C, P);
    if(qlast % 4 == 3)
        sqrtpow_q = sqrt_34(C, Q);
    else if(qlast % 8 == 5)
        sqrtpow_q = sqrt_58(C, Q);
    BigNum a1, b1;
    //use the extended euclidean algorithm to calculate a, b so ap+bq = 1;
    BigNum gcd = EEA(P, Q, a1, b1);
    BigNum APS = a1*P, BQR = b1*Q;
    //get x, -x, y, -y;
    APS = APS*sqrtpow_q;
    BQR = BQR*sqrtpow_p;
    BigNum X = APS+BQR, Y = APS-BQR;
    vector<BigNum> quorem = X/PQ;
    X = quorem[1];
    quorem = Y/PQ;
    Y = quorem[1];
    BigNum negX = PQ-X, negY = PQ-Y;
    quorem = negX/PQ;
    negX = quorem[1];
    quorem = negY/PQ;
    negY = quorem[1];
    vector<BigNum> roots = {X,negX, Y, negY};
    bool flag = true;
    BigNum plaintxt;
    //iterate over the 4 results, check for the padding for the plaintext
    for(auto &i: roots){
        flag = true;
        vector<uint8_t> dt = i.getdata();
        int l = dt.size();
        for(size_t j = 0; j < 2; j++){
            if(dt[l-j-1] != dt[l-j-3]){
                flag = false;
            }     
        }
        if(flag){
            plaintxt = i;
            break;
        }
    }
    s = plaintxt.tostring();
    s = s.substr(0, s.length()-4);
    while(s.length()<56){
        s = "0"+s;
    }
    return s;
}
int main(){
    srand(time(NULL));
    cout << "<Miller-Rabin>" << endl;
    string s = genBigPrime().tostring();
    printstring(s);
    cout << "<Rabin Encryption>" << endl;
    string  decryptP="",decryptQ="", ciphertext;
    string encryptedstr, decryptedstr;
    encryptedstr = RabinEncyption();
    cout << "Ciphertext = ";
    printstring(encryptedstr);
    cout << "<Rabin Decryption>" << endl;
    decryptedstr = RabinDecryption();
    cout << "Plaintext = ";
    printstring(decryptedstr);
}