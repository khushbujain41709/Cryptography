#include <bits/stdc++.h>
using namespace std;
// Extended Euclidean Algorithm
int modularInverse(int b, int a, int t1, int t2){
    if(b == 0) return t1;
    int q = a / b;
    int r = a % b;
    int t = t1 - (t2 * q);
    return modularInverse(r, b, t2, t);
}
int main(){
    cout<<"Name: Khushbu Jain"<<endl;
    cout<<"Roll Number: 23115047"<<endl;
    int n;
    cout << "Enter number of equations: ";
    cin >> n;
    vector<int> m(n), a(n);
    cout << "Enter relatively prime moduli and their remainders:\n";
    for(int i = 0; i < n; i++){
        cin >> m[i] >> a[i];
    }
    long long M = 1;
    for(int i = 0; i < n; i++){
        M *= (long long)m[i];
    }
    vector<long long> M_moduli(n), M_moduli_inverse(n);
    for(int i = 0; i < n; i++){
        M_moduli[i] = M / m[i];
        int inv = modularInverse(M_moduli[i], m[i], 0, 1);
        M_moduli_inverse[i] = (inv % m[i] + m[i]) % m[i];
    }
    long long x = 0;
    for(int i = 0; i < n; i++){
        x += (long long)a[i] * M_moduli_inverse[i] * M_moduli[i];
    }
    x = (x % M + M) % M;
    cout << "The integer that satisfies the given set of congruent equations is: "<< x << endl;
    return 0;
}
/*
Enter number of equations: 3
Enter relatively prime moduli and their remainders:
3 2
5 3
7 2
The integer that satisfies the given set of congruent equations is: 23
*/