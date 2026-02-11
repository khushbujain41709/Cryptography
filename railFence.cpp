// Rail Fence
#include<bits/stdc++.h>
using namespace std;
int main(){
    cout<<"Enter any capital string : ";
    string s;
    getline(cin, s);
    int depth;
    cout<<"Enter the value of depth : ";
    cin>>depth;
    if(depth <= 1){
        cout<<"Encrypted Text : "<<s<<endl;
        cout<<"Decrypted Text : "<<s<<endl;
        return 0;
    }
    int n = s.length();
    // Encryption
    vector<string> rail(depth);
    int row = 0;
    bool down = true;
    for(int i = 0; i<n; i++){
        rail[row] += s[i];
        if(row == 0){
            down = true;
        }
        else if(row == depth - 1){
            down = false;
        }
        row += (down ? 1 : -1);
    }
    string cipher = "";
    for(int i = 0; i<depth; i++){
        cipher += rail[i];
    }
    cout<<"Encrypted Text : "<<cipher<<endl;

    // Decryption
    vector<vector<char>> mat(depth, vector<char>(n, '\n'));
    // Step 1: Mark zig-zag positions
    row = 0;
    down = true;
    for(int i = 0; i < n; i++){
        mat[row][i] = '*';
        if(row == 0){
            down = true;
        }
        else if(row == depth - 1){
            down = false;
        }
        row += (down ? 1 : -1);
    }
    // Step 2: Fill marked positions with cipher text
    int idx = 0;
    for(int i = 0; i<depth; i++){
        for(int j = 0; j<n; j++){
            if(mat[i][j] == '*' && idx < n){
                mat[i][j] = cipher[idx++];
            }
        }
    }
    // Step 3: Read in zig-zag manner
    string plain = "";
    row = 0;
    down = true;
    for(int i = 0; i<n; i++){
        plain += mat[row][i];
        if(row == 0){
            down = true;
        }
        else if(row == depth - 1){
            down = false;
        }
        row += (down ? 1 : -1);
    }
    cout<<"Decrypted Text : "<<plain<<endl;
    return 0;
}