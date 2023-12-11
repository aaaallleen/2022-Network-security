#include <bits/stdc++.h>
#include "GF256.h"
#include "IO.h"
#include "AES.h"
using namespace std;
 //this is for printing each individual byte in the vector
void print(uint8_t a){
   
    if(a < 0x10)
        cout <<"0" << hex << a-0;
    else
        cout << hex << a-0 ;
}

//an overload function to print a whole vector, calls print(uint8_t) to print individual values
void print(vector<uint8_t> v){
    
    for(auto i: v){
        print(i);
        cout <<" ";
    }
    cout << endl;
    return;
}

//the GF256_mult and GF256_add are implemented in the header files as inline functions, but the implementation is commented below:
//inline uint8_t GF256_add(uint8_t a,uint8_t b) { return (a^b);}
//inline uint8_t GF256_mult_x(uint8_t a, const uint8_t mx) {return (a>=0x80)?((a<<1)^0x1b):(a<<1);}

//implement the mult as doing x^i i times and summing them up
uint8_t GF256_mult(uint8_t a, uint8_t b, const uint8_t mx){
    int arr[8] ={0}, i = 7;
    uint8_t c = b;
    //read each bit
    while(c > 0){
        arr[i] = c%2;
        c >>= 1;
        --i;
    }
    uint8_t tmpresult[8] = {0x00};
    //don't need to do mult for last bit
    if(arr[7] == 1)
        tmpresult[7] = a;
    for(int i = 6 ; i>=0; --i){
        if(arr[i] == 0)
            continue;
        else{
            tmpresult[i] = a;
            for(int j = 7; j > i; --j){
                tmpresult[i]= GF256_mult_x(tmpresult[i], mx);  
            }
        }
    }
    uint8_t result = 0x00;
    for(int i = 0; i < 8; i++){
        result = GF256_add(result, tmpresult[i]);
    }
    return result;
}

//to count deg of function
int poly_deg(uint8_t poly){
    int deg = -1;
    while(poly != 0){
        poly >>= 1;
        ++deg;
    }
    return deg;
}

//function to perform division between two 8bit streams, since we can only pass one value back, the remainder has to be modified through reference
uint8_t GF256_div(uint8_t a, uint8_t b, uint8_t &rem){
    uint8_t result, rb;
    int degA, degB, degRem;
    rb = b;
    degA = poly_deg(a);
    degB = poly_deg(rb);
    //return 0 for quotient and a for remainder if b is greater than a
    if(degA < degB){
        result = 0x00;
        rem = a;
    }
    else if(degA == degB){
        result = 0x01;
        rem = GF256_add(a, rb);
    }
    else{
        rem = a;
        degRem = degA;
        //perform shift on divisor, and add 2^shift to the quotient, and the rem would be modified with reference
        int quo = 0;
        while(degRem >= degB){
            int shift = degRem - degB;
            rb <<= shift;
            quo += pow(2,shift);
            rem = GF256_add(rb, rem);
            degRem = poly_deg(rem);
            rb = b;
        }
        assert((degB <= poly_deg(rem), "The rem has greater/equal deg, error!"));
        result = quo;
    }  
    return result;
}

//perform extended GCD to the number 
uint8_t GF256_ext_gcd(uint8_t a, const uint8_t mx){
    // this part is to handle mx since mx has deg of 8
    int r_mx = mx+0x100;
    int shift = 8 - poly_deg(a);
    uint8_t quo = pow(2, shift);
    int tmp = (a << shift);
    uint8_t rem, nrem, tmprem;
    quo += GF256_div((tmp^r_mx), a, rem);
    // then we perform looped version of extended_euclidean
    uint8_t t0 = 0x00, t1 = 0x01;
    uint8_t t = t0^(GF256_mult(t1, quo, mx));

    while(rem != 0){
        quo = GF256_div( a, rem, nrem);
        t0 = t1;
        t1 = t;
        t = t0^(GF256_mult(t1,quo, mx));
        a = rem;
        rem = nrem;
    }
    return t1;
    cout << "exit loop" << endl;
}

//inverse calls GF256_ext_gcd but handles cases such as 0x00 and 0x01
uint8_t GF256_inv(uint8_t a, const uint8_t mx){
    if(a == 0x00)
        return 0x00;
    if(a == 0x01) 
        return 0x01;
    uint8_t result =  GF256_ext_gcd(a, mx);
    // cout << "Deg of a is " << poly_deg(a) << " Deg of mx is " << poly_deg(mx) << endl;
    return result;
}

//Sub_byte function
vector<uint8_t> Sub_Byte(vector<uint8_t> v, const uint8_t mx){
    vector<uint8_t> result, invResult;

    //find the inverse first
    for(auto i: v){
        invResult.push_back(GF256_inv(i, mx));
    }
    
    for(auto byte: invResult){
        uint8_t tmp= 0b00000000, tmpbit;
        //isolate each byte in the uint8_t number
        int bits[8], resultbits[8];
        for(int i = 0; i < 8; ++i){
            bits[i] = byte%2;
            byte >>= 1;
        }
        //matrix multiplication 
        resultbits[0] = bits[0]^bits[4]^bits[5]^bits[6]^bits[7];
        resultbits[1] = bits[0]^bits[1]^bits[5]^bits[6]^bits[7];
        resultbits[2] = bits[0]^bits[1]^bits[2]^bits[6]^bits[7];
        resultbits[3] = bits[0]^bits[1]^bits[2]^bits[3]^bits[7];
        resultbits[4] = bits[0]^bits[1]^bits[2]^bits[3]^bits[4];
        resultbits[5] = bits[1]^bits[2]^bits[3]^bits[4]^bits[5];
        resultbits[6] = bits[2]^bits[3]^bits[4]^bits[5]^bits[6];
        resultbits[7] = bits[3]^bits[4]^bits[5]^bits[6]^bits[7];

        
        for( int i = 7; i >= 0 ; --i){
            if(resultbits[i]){
                tmp <<= 1;
                tmp += 1;
            }
            else
                tmp <<= 1;
        }
        //add the "11000110" to the word
        tmp ^= 0b01100011;

        result.push_back(tmp);
    }
    return result;
}

//shift_rows function
vector<uint8_t> Shift_Rows(vector<uint8_t> v){
    vector<uint8_t> result;
    result.push_back(v[0]);
    result.push_back(v[5]);
    result.push_back(v[10]);
    result.push_back(v[15]);

    result.push_back(v[4]);
    result.push_back(v[9]);
    result.push_back(v[14]);
    result.push_back(v[3]);

    result.push_back(v[8]);
    result.push_back(v[13]);
    result.push_back(v[2]);
    result.push_back(v[7]);

    result.push_back(v[12]);
    result.push_back(v[1]);
    result.push_back(v[6]);
    result.push_back(v[11]);
    return result;
}

//mix_columns function
vector<uint8_t> Mix_Columns(vector<uint8_t> v, const uint8_t mx){
    vector<uint8_t> result;
    for(auto i = 0; i < 16; i+=4){
        uint8_t tmpresult;
        tmpresult = (GF256_mult(0x02, v[i], mx) ^ GF256_mult(0x03, v[i+1], mx) ^ v[i+2] ^ v[i+3]);
        result.push_back(tmpresult);
        tmpresult = (v[i] ^ GF256_mult(0x02, v[i+1], mx) ^ GF256_mult(0x03, v[i+2], mx) ^ v[i+3]);
        result.push_back(tmpresult);
        tmpresult = (v[i] ^ v[i+1] ^ GF256_mult(0x02, v[i+2], mx) ^ GF256_mult(0x03, v[i+3], mx));
        result.push_back(tmpresult);
        tmpresult = (GF256_mult(0x03, v[i], mx) ^ v[i+1] ^ v[i+2] ^ GF256_mult(0x02, v[i+3], mx));
        result.push_back(tmpresult);
    }
    return result;
}

//generate the round keys
vector<vector<uint8_t>> Gen_Roundkey(vector<uint8_t> v, const uint8_t mx){
    vector<uint8_t> rcon, prevWord, prevRound = v;

    vector<vector<uint8_t>> result;
    //push in the original key for the round 0 pre-process
    result.push_back(v);
    
    //form the rcon vector
    rcon.push_back(0x01);
    rcon.push_back(0x02);
    rcon.push_back(0x04);
    rcon.push_back(0x08);
    rcon.push_back(0x10);
    rcon.push_back(0x20);
    rcon.push_back(0x40);
    rcon.push_back(0x80);
    rcon.push_back(0x1b);
    rcon.push_back(0x36);

    //push the last word in the key to be process into the new t
    for(auto i = 0; i < 10; ++i){
        vector<uint8_t> Wx;
        uint8_t tmpword;
        for(auto j = 0 ; j<4; ++j ){
            if( j == 0){
                //with each round, we have to rot word then sub word then add rcon to form t
                prevWord.clear();
                prevWord.push_back(prevRound[13]);
                prevWord.push_back(prevRound[14]);
                prevWord.push_back(prevRound[15]);
                prevWord.push_back(prevRound[12]);
                prevWord = Sub_Byte(prevWord, mx);

                //the xor for each byte
                tmpword = prevRound[j*4]^prevWord[0]^rcon[i];
                Wx.push_back(tmpword);
                prevWord[0] = tmpword; 

                tmpword = prevRound[j*4+1]^prevWord[1];
                Wx.push_back(tmpword);
                prevWord[1] = tmpword;

                tmpword = prevRound[j*4+2]^prevWord[2];
                Wx.push_back(tmpword);
                prevWord[2] = tmpword;

                tmpword = prevRound[j*4+3]^prevWord[3];
                Wx.push_back(tmpword);
                prevWord[3] = tmpword;

            }
            else{
                //the xor for each byte
                tmpword = prevRound[j*4]^prevWord[0];
                Wx.push_back(tmpword);
                prevWord[0] = tmpword; 

                tmpword = prevRound[j*4+1]^prevWord[1];
                Wx.push_back(tmpword);
                prevWord[1] = tmpword;

                tmpword = prevRound[j*4+2]^prevWord[2];
                Wx.push_back(tmpword);
                prevWord[2] = tmpword;

                tmpword = prevRound[j*4+3]^prevWord[3];
                Wx.push_back(tmpword);
                prevWord[3] = tmpword;

            }
            
        }
        prevRound = Wx;
        result.push_back(Wx);
        Wx.clear();
    }
    return result;
}

//the addround keys for the texts
vector<uint8_t> Add_Roundkeys(vector<uint8_t> v1, vector<uint8_t> v2){
    vector<uint8_t> result;
    for(auto i = 0; i< 16; ++i)
        result.push_back(v1[i]^v2[i]);
    return result;
}
vector<uint8_t> vectorize_input(string s){
    vector<uint8_t> result;
    for(auto i = 0; i < s.size(); i+=2){
        string hex_pair = s.substr(i, 2);
        result.push_back(stoul(hex_pair, nullptr, 16));
    }
    return result;
}

void AES_Encrypt(string plaintext, string key){

    //the input is a string for both plaintext and key, we do this to make it into an array of uint8_t in form hex
    vector<uint8_t> v_plaintext = vectorize_input(plaintext);
    vector<uint8_t> v_key = vectorize_input(key);
    //we generate the round keys for each round and store the, into a vector, the data structure would result in a vector<vector<uint8_t>>
    vector<vector<uint8_t>> roundKey = Gen_Roundkey(v_key, mx);
    //roundresult is a temporary storage for the intermediate states
    vector<uint8_t> roundResult;
    //round 0 for the pre-round process, round 10 is a little different since it does not mix_collumns
    for(auto i = 0; i <= 10; i++){
        if(i == 0){
            roundResult = Add_Roundkeys(v_plaintext, roundKey[i]);
            continue;
        }    
        else if (i == 10){
            roundResult = Sub_Byte(roundResult, mx);
            roundResult = Shift_Rows(roundResult);
            roundResult = Add_Roundkeys(roundResult,roundKey[i]);
        }
        else{
            roundResult = Sub_Byte(roundResult, mx);
            roundResult = Shift_Rows(roundResult);
            roundResult = Mix_Columns(roundResult, mx);
            roundResult = Add_Roundkeys(roundResult,roundKey[i]);
        }
        cout << "Round " <<dec << i << ":"<< endl;
        print(roundResult);
    }
}
int main(){
    
    string plaintext, key;
    cout << "Plaintext : ";
    cin >> plaintext;
    cout << "Key : ";
    cin >> key;

    //calls AES_Encrypt
    AES_Encrypt(plaintext, key);
 
}