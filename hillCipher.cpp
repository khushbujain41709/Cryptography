// Hill cipher
#include<bits/stdc++.h>
using namespace std;
// Convert key into n*n matrix where n is given by user and plain text is partitioned into n parts
vector<vector<int>> multiply(vector<vector<int>> &A, vector<vector<int>> &B) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();
    vector<vector<int>> C(n, vector<int>(p, 0));
    for(int i = 0; i<n; i++){
        for(int j = 0; j<p; j++){
            for(int k = 0; k<m; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
            C[i][j] = (C[i][j] % 26 + 26) % 26;
        }
    }
    return C;
}
vector<vector<int>> inverseMatrix(vector<vector<int>>& keyMat){
    int n = keyMat.size();
    
}
int main(){
    // A - 0, Z - 25
    cout<<"Enter size of matrix : ";
    int n;
    cin>>n;
    // Let us assume that plain text has no whitespace
    cout<<"Enter any capital string : ";
    string s;
    cin>>s;
    cout<<"Enter key matrix : ";
    vector<vector<int>> keyMat(n, vector<int>(n));
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            cin>>keyMat[i][j];
        }
    }
    // Encryption
    string cipher = "";
    // Process plaintext in blocks of size n
    int blocks = s.size()/n;
    vector<vector<int>> stringMat(n, vector<int>(blocks));
    int idx = 0;
    for(int j = 0; j<blocks; j++){ // j then i - fill column wise
        for(int i = 0; i<n; i++){
            stringMat[i][j] = s[idx++] - 'A';
        }
    }
    vector<vector<int>> result = multiply(keyMat, stringMat);
    for(int i = 0; i<result.size(); i++){
        for(int j = 0; j<result[0].size(); j++){
            cipher += (char)(result[i][j] % 26) + 'A';
        }
    }
    cout<<"Encrypted Text : "<<cipher<<endl;

    // Decryption
    string plain = "";
    vector<vector<int>> invKey = inverseMatrix(keyMat);
    vector<vector<int>> plainMat = multiply(invKey, result);
    for(int i = 0; i<plainMat.size(); i++){
        for(int j = 0; j<plainMat[0].size(); j++){
            plain += (char)(plainMat[i][j] % 26) + 'A';
        }
    }
    cout<<"Decrypted Text : "<<plain<<endl;
    return 0;

}
