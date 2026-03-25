#include <bits/stdc++.h>
using namespace std;
// Fast Modular Exponentiation
long long power(long long a, long long b, long long m){
    long long res = 1;
    a = a % m;
    while(b > 0){
        if(b & 1)
            res = (res * 1LL * a) % m;

        a = (a * 1LL * a) % m;
        b = b / 2;
    }
    return res;
}
// Extended Euclidean Algorithm for Modular Inverse
int modInverse(int b, int a, int t1, int t2){
    if(b == 0) return t1;
    int q = a / b;
    int r = a % b;
    int t = t1 - (q * t2);
    return modInverse(r, b, t2, t);
}
int main(){
    cout<<"Name: Khushbu Jain"<<endl;
    cout<<"Roll Number: 23115047"<<endl;
    
    int p, q;
    cout<<"Enter any two prime numbers: ";
    cin>>p>>q;

    int n = p * q;
    int phi_n = (p - 1) * (q - 1);

    int e;
    cout << "Enter Public key e (GCD(e, phi_n) must be 1 and 1 < e < phi_n): ";
    cin >> e;

    // Compute private key d
    int d = modInverse(e, phi_n, 0, 1);
    d = (d % phi_n + phi_n) % phi_n;  // make positive
    cout<<"Private key d: "<<d<<endl;

    int M;
    cout<<"Enter message: ";
    cin>>M;

    // Encryption: c = M^e mod n
    int cipher_text = power(M, e, n);
    cout<<"Generated cipher text is: "<<cipher_text<<endl;

    // Decryption: p = c^d mod n
    int plain_text = power(cipher_text, d, n);
    cout<<"Decrypted plain text is: "<<plain_text<<endl;

    return 0;
}