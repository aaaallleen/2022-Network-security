#include <bits/stdc++.h>
using namespace std;
int main(){
    srand(5);
    char c[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    string plaintext = "";
    string key = "";
    for(int i = 0; i < 32; i++){
        plaintext+=c[rand()%16];
        key+=c[rand()%16];
    }
    cout << plaintext << endl;
    cout << key << endl;
}