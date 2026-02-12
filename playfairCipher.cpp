// Playfair Cipher
#include<bits/stdc++.h>
using namespace std;
int main(){
    // A - 0, Z - 25
    // Assume no whitespaces in plaintext
    // Filler letter is : X and if XX will be the diagram, choose filler letter as Q
    cout<<"Enter any capital string : ";
    string s;
    cin>>s;
    cout<<"Enter any capital key : ";
    string key;
    cin>>key;
    unordered_set<char> ms, mk;
    for(int i = 0; i<key.size(); i++){
        mk.insert(key[i]);
    }
    for(int i = 0; i<s.size(); i++){
        ms.insert(s[i]);
    }
    // 5*5 matrix formation
    vector<vector<char>> v(5, vector<char>(5, ' '));
    vector<bool> used(26, false);
    used['J' - 'A'] = true;  // Skip J
    string full = "";
    // Add key without duplicates
    for(int i = 0; i < key.size(); i++){
        if(!used[key[i] - 'A']){
            used[key[i] - 'A'] = true;
            full += key[i];
        }
    }
    // Add remaining characters
    for(char c = 'A'; c <= 'Z'; c++){
        if(!used[c - 'A']){
            used[c - 'A'] = true;
            full += c;
        }
    }
    // Fill matrix
    int idx = 0;
    for(int i = 0; i<5; i++){
        for(int j = 0; j<5; j++){
            v[i][j] = full[idx++];
        }
    }
    // Generating Diagrams
    vector<string> diagrams;
    for(int i = 0; i<s.size(); i++){
        if(i == s.size()-1){
            if(s[i] == 'X') diagrams.push_back({s[i], 'Q'});
            else diagrams.push_back({s[i], 'X'});
        }
        else if(s[i] != s[i+1]){
            diagrams.push_back({s[i],s[i+1]});
            i++;
        }
        else{
            if(s[i] == 'X') diagrams.push_back({s[i], 'Q'});
            else diagrams.push_back({s[i], 'X'});
        }
    }
    // for(int i = 0; i<5; i++){
    //     for(int j = 0; j<5; j++){
    //         cout<<v[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // for(int i = 0; i<diagrams.size(); i++){
    //     cout<<diagrams[i]<<" ";
    // }

    // Encryption
    string cipher = "";
    for(int i = 0; i<diagrams.size(); i++){
        int c1 = diagrams[i][0];
        int c2 = diagrams[i][1];
        int r1, r2, c1_pos, c2_pos;
        for(int i = 0; i<5; i++){
            for(int j = 0; j<5; j++){
                if(v[i][j] == c1){
                    r1 = i;
                    c1_pos = j;
                }
                if(v[i][j] == c2){
                    r2 = i;
                    c2_pos = j;
                }
            }
        }
        // Same row
        if(r1 == r2){
            cipher += v[r1][(c1_pos + 1) % 5];
            cipher += v[r2][(c2_pos + 1) % 5];
        }
        // Same column
        else if(c1_pos == c2_pos){
            cipher += v[(r1 + 1) % 5][c1_pos];
            cipher += v[(r2 + 1) % 5][c2_pos];
        }
        // Rectangle rule
        else{
            cipher += v[r1][c2_pos];
            cipher += v[r2][c1_pos];
        }
    }
    cout<<"Encrypted Text : "<<cipher<<endl;

    // Decryption
    string plain = "";
    for(int i = 0; i<cipher.size(); i+=2){
        int c1 = cipher[i];
        int c2 = cipher[i+1];
        int r1, r2, c1_pos, c2_pos;
        for(int i = 0; i<5; i++){
            for(int j = 0; j<5; j++){
                if(v[i][j] == c1){
                    r1 = i;
                    c1_pos = j;
                }
                if(v[i][j] == c2){
                    r2 = i;
                    c2_pos = j;
                }
            }
        }
        // Same row
        if(r1 == r2){
            plain += v[r1][((c1_pos - 1)+5) % 5];
            plain += v[r2][((c2_pos - 1)+5) % 5];
        }
        // Same column
        else if(c1_pos == c2_pos){
            plain += v[((r1 - 1 )+5) % 5][c1_pos];
            plain += v[((r2 - 1)+5) % 5][c2_pos];
        }
        // Rectangle rule
        else{
            plain += v[r1][c2_pos];
            plain += v[r2][c1_pos];
        }
    }
    cout<<"Decrypted Text : "<<plain<<endl;
    // Because Playfair loses information.
    /*
    Enter any capital string : INSTRUMENTS
    Enter any capital key : MONARCHY
    Encrypted Text : GATLMZCLRQXA
    Decrypted Text : INSTRUMENTSX
    */
    return 0;
}