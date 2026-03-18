#include<bits/stdc++.h>
using namespace std;
// Implementation of Diffie Hellman Key Exchange

// Power function to return value of a^b mod P
long long power(long long a, long long b, long long P){
    long long result = 1;
    a = a % P;
    while(b > 0){
        if(b & 1){
            result = (result * a) % P;
        }
        a = (a * a) % P;
        b >>= 1;
    }
    return result;
}
int main(){
    cout<<"Name: Khushbu Jain"<<endl;
    cout<<"Roll Number: 23115047"<<endl;

    long long P, G, x, a, y, b, ka, kb;
    long long ma, mb, mx, my, kma, kmb;

    cout<<"Enter the value of prime number : ";
    cin>>P;
    cout<<"The value of P : "<<P<<endl;
    cout<<"Enter the value of Generator: ";
    cin>>G;
    cout<<"The value of G : "<<G<<endl;

    cout<<"Enter the private key for Alice: ";
    cin>>a;
    cout<<"The private key for Alice : "<<a<<endl;
    x = power(G, a, P);   // gets the generated key
    cout<<"Enter the private key for Bob: ";
    cin>>b;
    cout<<"The private key for Bob : "<<b<<endl;
    y = power(G, b, P);

    // Man-in-the-middle attack
    cout<<"Enter the private key for Man-in-the-middle shared with Alice : ";
    cin>>ma;
    cout<<"The private key for Man-in-the-middle shared with Alice : "<<ma<<endl;
    mx = power(G, ma, P);
    cout<<"Enter the private key for Man-in-the-middle shared with Bob : ";
    cin>>mb;
    cout<<"The private key for Man-in-the-middle shared with Bob : "<<mb<<endl;
    my = power(G, mb, P);

    // Generating the secret key after the exchange of keys
    ka = power(mx, a, P);
    kb = power(my, b, P);
    kma = power(x, ma, P);
    kmb = power(y, mb, P);
    cout<<"Secret key for the Alice is : "<<ka<<endl;
    cout<<"Secret key for the Man-in-the-Middle shared with Alice is : "<<kma<<endl;
    cout<<"Secret key for the Bob is : "<<kb<<endl;
    cout<<"Secret key for the Man-in-the-Middle shared with Bob is : "<<kmb<<endl;
    return 0;
}
/*
Output:
Enter the value of prime number : 23
The value of P : 23
The private key for Alice : 6
Enter the private key for Bob: 15
The private key for Bob : 15
Enter the private key for Man-in-the-middle shared with Alice : 3
The private key for Man-in-the-middle shared with Alice : 3
Enter the private key for Man-in-the-middle shared with Bob : 7
The private key for Man-in-the-middle shared with Bob : 7
Secret key for the Alice is : 6
Secret key for the Man-in-the-Middle shared with Alice is : 6
Secret key for the Bob is : 15
The private key for Alice : 6
Enter the private key for Bob: 15
th Alice : 3
Enter the private key for Man-in-the-middle shared with Bob : 7        
The private key for Man-in-the-middle shared with Bob : 7
Secret key for the Alice is : 6
Secret key for the Man-in-the-Middle shared with Alice is : 6
Secret key for the Bob is : 15
Secret key for the Man-in-the-Middle shared with Bob is : 15
*/