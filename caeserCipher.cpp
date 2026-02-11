// Caeser Cipher
#include<iostream>
#include<vector>
using namespace std;
int main(){
    string s;
    cout<<"Enter any capital string : ";
    cin>>s;
    // A - 0, Z - 25
    string plain = "";
    string cipher = "";
    int key = 3;
    int encrypt = 0;
    int decrypt = 0;
    for(int i = 0; i<s.size(); i++){
        if(s[i] == ' ') continue;
        encrypt = ((s[i] - 'A') + key) % 26;
        if(encrypt >= 0){
            plain += (char)encrypt + 'A';
        }
        else{
            plain += (char)(26 + encrypt)  + 'A';
        }
    }
    for(int i = 0; i<s.size(); i++){
        decrypt = ((plain[i] - 'A') - key) % 26;
        if(decrypt >= 0){
            cipher += (char)decrypt + 'A';
        }
        else{
            cipher += (char)(26 + decrypt)  + 'A';
        }
    }
    cout<<"Encrypted code is : "<<plain<<endl;
    cout<<"Decrypted code is : "<<cipher<<endl;
    return 0;
}
