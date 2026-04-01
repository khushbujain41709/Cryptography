#include <iostream>
#include <utility>
using namespace std;
typedef long long ll;
// Global parameters
ll p, a, b;
// Modular Inverse
ll modinv(ll k){
    k = (k % p + p) % p;
    ll res = 1, exp = p - 2;
    while (exp) {
        if (exp & 1) res = (res * k) % p;
        k = (k * k) % p;
        exp >>= 1;
    }
    return res;
}
// Point Addition
pair<ll,ll> point_add(pair<ll,ll> P, pair<ll,ll> Q){
    // Identity cases
    if (P.first == -1) return Q;
    if (Q.first == -1) return P;
    ll x1 = P.first, y1 = P.second;
    ll x2 = Q.first, y2 = Q.second;
    // P + (-P) = infinity
    if (x1 == x2 && (y1 + y2) % p == 0)
        return make_pair(-1, -1);
    ll m;
    if(P == Q){
        // Point doubling
        ll numerator = (3 * x1 * x1 + a) % p;
        ll denominator = modinv(2 * y1);
        m = (numerator * denominator) % p;
    } 
    else{
        // Normal addition
        ll numerator = (y2 - y1 + p) % p;
        ll denominator = modinv(x2 - x1 + p);
        m = (numerator * denominator) % p;
    }
    ll x3 = (m * m - x1 - x2) % p;
    ll y3 = (m * (x1 - x3) - y1) % p;
    x3 = (x3 + p) % p;
    y3 = (y3 + p) % p;
    return make_pair(x3, y3);
}
// Point Negation
pair<ll,ll> negate_point(pair<ll,ll> P){
    if (P.first == -1) return P;
    ll x = P.first;
    ll y = (-P.second % p + p) % p;
    return make_pair(x, y);
}
// Scalar Multiplication
pair<ll,ll> scalar_mult(ll k, pair<ll,ll> P){
    pair<ll,ll> R = make_pair(-1, -1); // infinity
    while (k){
        if(k & 1) R = point_add(R, P);
        P = point_add(P, P);
        k >>= 1;
    }
    return R;
}
// Check Point
bool is_on_curve(pair<ll,ll> P){
    ll x = P.first, y = P.second;
    return ((y*y) % p == ( (x*x*x + a*x + b) % p ));
}
int main(){
    cout<<"Name: Khushbu Jain"<<endl;
    cout<<"Roll Number: 23115047"<<endl;

    cout << "Enter prime p: ";
    cin >> p;

    cout << "Enter a and b: ";
    cin >> a >> b;

    pair<ll,ll> G;
    cout << "Enter base point G (x y): ";
    cin >> G.first >> G.second;

    if(!is_on_curve(G)){
        cout << "Error: G not on curve!\n";
        return 0;
    }

    ll priv;
    cout << "Enter private key: ";
    cin >> priv;

    pair<ll,ll> M;
    cout << "Enter message point M (x y): ";
    cin >> M.first >> M.second;

    if(!is_on_curve(M)){
        cout << "Error: Message not on curve!\n";
        return 0;
    }

    // Public key
    pair<ll,ll> pub = scalar_mult(priv, G);
    cout << "\nPublic Key: (" << pub.first << ", " << pub.second << ")\n";

    // Encryption
    ll k;
    cout << "Enter random k: ";
    cin >> k;

    pair<ll,ll> C1 = scalar_mult(k, G);
    pair<ll,ll> shared = scalar_mult(k, pub);
    pair<ll,ll> C2 = point_add(M, shared);

    cout << "\nEncrypted:\n";
    cout << "C1 = (" << C1.first << ", " << C1.second << ")\n";
    cout << "C2 = (" << C2.first << ", " << C2.second << ")\n";

    // Decryption
    pair<ll,ll> shared2 = scalar_mult(priv, C1);
    pair<ll,ll> neg = negate_point(shared2);
    pair<ll,ll> M_dec = point_add(C2, neg);

    cout << "\nDecrypted Message Point: ("
         << M_dec.first << ", " << M_dec.second << ")\n";

    return 0;
}
/*
Name: Khushbu Jain
Roll Number: 23115047
Enter prime p: 11
Enter a and b: 1 6
Enter base point G (x y): 2 7
Enter private key: 3
Enter message point M (x y): 10 9

Public Key: (8, 3)
Enter random k: 2

Encrypted:
C1 = (5, 2)
C2 = (5, 2)

Decrypted Message Point: (10, 9)
*/