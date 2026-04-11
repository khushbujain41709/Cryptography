#include <iostream>
using namespace std;
// Fast modular exponentiation
long long power(long long base, long long exp, long long mod){
    long long res = 1;
    base %= mod;
    while(exp > 0){
        if (exp % 2 == 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

// Extended Euclidean Algorithm
long long extGCD(long long a, long long b, long long &x, long long &y){
    if(b == 0){
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long g = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
// Modular inverse
long long modInverse(long long a, long long m){
    long long x, y;
    long long g = extGCD(a, m, x, y);
    if(g != 1){
        cout << "Modular inverse does not exist!" << endl;
        return -1;
    }
    return (x % m + m) % m;
}
int main(){
    long long p, e1, d;
    cout << "Enter value of p: ";
    cin >> p;
    cout << "Enter value of e1: ";
    cin >> e1;
    cout << "Enter private key (d): ";
    cin >> d;
    long long e2 = power(e1, d, p);
    cout << endl;
    cout << "Key Generation:" << endl;
    cout << "Public Key (e1, e2, p): " << e1 << ", " << e2 << ", " << p << endl;
    cout << "Private Key (d): " << d << endl << endl;
    long long M, r;
    cout << "Enter plaintext (M): ";
    cin >> M;
    cout << "Enter random r: ";
    cin >> r;
    long long C1 = power(e1, r, p);
    long long C2 = (power(e2, r, p) * M) % p;
    cout << endl;
    cout << "Ciphertext:" << endl;
    cout << "C1: " << C1 << endl;
    cout << "C2: " << C2 << endl << endl;
    long long C1d = power(C1, d, p);
    long long inv = modInverse(C1d, p);
    long long decrypted = (C2 * inv) % p;
    cout << "Decryption:" << endl;
    cout << "Decrypted message: " << decrypted << endl;
    if(decrypted == M){
        cout << "SUCCESS: Decryption is correct." << endl;
    } 
    else{
        cout << "FAILURE: Decryption is incorrect." << endl;
    }
    return 0;
}
/*
Enter value of p: 19
Enter value of e1: 10
Enter private key (d): 5

Key Generation:
Public Key (e1, e2, p): 10, 3, 19
Private Key (d): 5

Enter plaintext (M): 17
Enter random r: 6

Ciphertext:
C1: 11
C2: 5

Decryption:
Decrypted message: 17
SUCCESS: Decryption is correct.
*/