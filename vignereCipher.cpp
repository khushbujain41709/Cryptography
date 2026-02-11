// Vignere Cipher
#include<bits/stdc++.h>
using namespace std;
int main(){
    // A - 0, Z - 25
    cout<<"Enter any capital string : ";
    string s;
    getline(cin, s);
    cout<<"Enter any capital key : ";
    string key;
    cin>>key;
    int count = 0;
    for(int i = 0; i<s.size(); i++){
        if(s[i] != ' '){
            count++;
        }
    }
    int diff = (count - key.size());
    int k = 0;
    if(diff > 0){
        for(int i = 0; i<diff; i++){
            key.push_back(key[k]);
            k++;
        }
    }
    // Encryption
    vector<int> m, p;
    int ki = 0;
    for(int i = 0; i<s.size(); i++){
        if(s[i] == ' '){
            continue;
        }
        p.push_back(s[i] - 'A');
        m.push_back(key[ki] - 'A');
        ki++;
    }
    string cipher = "";
    int idx = 0;
    for(int i = 0; i<s.size(); i++){
        if(s[i] == ' '){
            cipher += ' ';
        }
        else{
            int sum = (p[idx]+m[idx]) % 26;
            cipher += (sum + 'A');
            idx++;
        }
    }
    cout<<"Encrypted Text is : "<<cipher<<endl;
    // Decryption
    idx = 0;
    string plain = "";
    for(int i = 0; i<cipher.size(); i++){
        if(cipher[i] == ' '){
            plain += ' ';
        }
        else{
            int sub = ((cipher[i]-'A') - (key[idx]-'A')) % 26;
            if(sub >= 0){
                plain += (sub + 'A');
            }
            else{
                plain += (26+sub + 'A');
            }
            idx++;
        }
    }
    cout<<"Decrypted Text is : "<<plain<<endl;
    return 0;
}