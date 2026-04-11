#include <iostream>
using namespace std;
// Fast modular exponentiation
long long power(long long base, long long exp, long long mod){
    long long res = 1;
    base %= mod;
    while(exp > 0){
        if(exp % 2 == 1){
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}
// Modular inverse using Extended Euclidean Algorithm
long long modInverse(long long n, long long mod){
    long long m0 = mod, t, q;
    long long x0 = 0, x1 = 1;
    if(mod == 1) return 0;
    while(n > 1){
        q = n / mod;
        t = mod;
        mod = n % mod;
        n = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0){
        x1 += m0;
    }
    return x1;
}
int main(){
    cout<<"Name: Khushbu Jain"<<endl;
    cout<<"Roll Number: 23115047"<<endl;
    long long p, q, g, x;
    cout << "Enter value of p: ";
    cin >> p;
    cout << "Enter value of q: ";
    cin >> q;
    cout << "Enter value of g: ";
    cin >> g;
    cout << "Enter private key (x): ";
    cin >> x;
    long long y = power(g, x, p);
    cout << endl;
    cout << "Key Generation:" << endl;
    cout << "Public Key (y): " << y << endl;
    cout << "Private Key (x): " << x << endl << endl;
    long long hash_val, k;
    cout << "Enter hash value: ";
    cin >> hash_val;
    cout << "Enter random k: ";
    cin >> k;
    long long r = power(g, k, p) % q;
    long long k_inv = modInverse(k, q);
    long long s = (k_inv * (hash_val + (x * r))) % q;
    cout << endl;
    cout << "Digital Signature:" << endl;
    cout << "r: " << r << endl;
    cout << "s: " << s << endl << endl;
    long long w = modInverse(s, q);
    long long u1 = (hash_val * w) % q;
    long long u2 = (r * w) % q;
    long long v = ((power(g, u1, p) * power(y, u2, p)) % p) % q;
    cout << "Verification:" << endl;
    cout << "Verification value (v): " << v << endl;
    if (v == r){
        cout << "SUCCESS: Signature is valid." << endl;
    } 
    else{
        cout << "FAILURE: Signature is invalid." << endl;
    }
    return 0;
}
/*
Enter value of p: 23
Enter value of q: 11
Enter value of g: 2
Enter private key (x): 7

Key Generation:
Public Key (y): 13
Private Key (x): 7

Enter hash value: 9
Enter random k: 3

Digital Signature:
r: 8
s: 7

Verification:
Verification value (v): 8
SUCCESS: Signature is valid.
*/