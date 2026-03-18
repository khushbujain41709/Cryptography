#include <iostream>
#include <string>
#include <cmath>
using namespace std;
string round_keys[16];
string pt;
string convertDecimalToBinary(int decimal){
    string binary;
    while(decimal != 0){
        binary = (decimal % 2 == 0 ? "0" : "1") + binary;
        decimal = decimal/2;
    }
    while(binary.length() < 4){
        binary = "0" + binary;
    }
    return binary;
}

int convertBinaryToDecimal(string binary){
    int decimal = 0;
    int counter = 0;
    int size = binary.length();
    for(int i = size-1; i>=0; i--){
        if(binary[i] == '1'){
            decimal += pow(2, counter);
        }
        counter++;
    }
    return decimal;
}

string shift_left_once(string key_chunk){
    string shifted="";
    for(int i = 1; i<28; i++){
        shifted += key_chunk[i];
    }
    shifted += key_chunk[0];
    return shifted;
}

string shift_left_twice(string key_chunk){
    string shifted="";
    for(int i = 0; i<2; i++){
        for(int j = 1; j<28; j++){
            shifted += key_chunk[j];
        }
        shifted += key_chunk[0];
        key_chunk = shifted;
        shifted = "";
    }
    return key_chunk;
}

string Xor(string a, string b){
    string result = "";
    int size = b.size();
    for(int i = 0; i<size; i++){
        if(a[i] != b[i]){
            result += "1";
        }
        else{
            result += "0";
        }
    }
    return result;
}

void generate_keys(string key){
    int pc1[56] = {
        57,49,41,33,25,17,9,
        1,58,50,42,34,26,18,
        10,2,59,51,43,35,27,
        19,11,3,60,52,44,36,
        63,55,47,39,31,23,15,
        7,62,54,46,38,30,22,
        14,6,61,53,45,37,29,
        21,13,5,28,20,12,4
    };

    int pc2[48] = {
        14,17,11,24,1,5,
        3,28,15,6,21,10,
        23,19,12,4,26,8,
        16,7,27,20,13,2,
        41,52,31,37,47,55,
        30,40,51,45,33,48,
        44,49,39,56,34,53,
        46,42,50,36,29,32
    };

    string perm_key = "";
    for(int i = 0; i<56; i++){
        perm_key += key[pc1[i]-1];
    }
    string left = perm_key.substr(0, 28);
    string right = perm_key.substr(28, 28);
    
    for(int i = 0; i<16; i++){
        if(i == 0 || i == 1 || i==8 || i==15 ){
            left = shift_left_once(left);
            right = shift_left_once(right);
        }
        else{
            left = shift_left_twice(left);
            right = shift_left_twice(right);
        }
        string combined_key = left + right;
        string round_key = "";
        for(int i = 0; i<48; i++){
            round_key += combined_key[pc2[i]-1];
        }

        round_keys[i] = round_key;
    }
}

string textToBinary(string text){
    // Pad to 8 chars with spaces
    while (text.length() < 8){
        text += ' ';
    }
    if(text.length() > 8) text = text.substr(0, 8);
    string binary = "";
    for(char c : text){
        int decimal = (unsigned char)c;
        for(int i = 7; i >= 0; --i){
            binary += (decimal & (1 << i)) ? '1' : '0';
        }
    }
    return binary;
}

string binaryToText(string bin){
    string text = "";
    for(int i = 0; i<64; i += 8){
        string byte_bin = bin.substr(i, 8);
        int decimal = 0;
        for(int j = 0; j < 8; ++j){
            if(byte_bin[j] == '1'){
                decimal |= (1 << (7 - j));
            }
        }
        text += (char)decimal;
    }
    // Trim trailing spaces/null
    while (!text.empty() && text.back() == ' ') {
        text.pop_back();
    }
    return text;
}

string DES(){
    int initial_permutation[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };
    
    int expansion_table[48] = {
        32,1,2,3,4,5,4,5,
        6,7,8,9,8,9,10,11,
        12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,
        22,23,24,25,24,25,26,27,
        28,29,28,29,30,31,32,1
    };
    
    int substitution_boxes[8][4][16] = {{
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },{
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },{
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },{
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },{
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },{
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },{
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },{
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }};

    int permutation_tab[32] = {
        16,7,20,21,29,12,28,17,
        1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,
        19,13,30,6,22,11,4,25
    };

    int inverse_permutation[64] = {
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
    };

    string perm = "";
    for(int i = 0; i<64; i++){
        perm += pt[initial_permutation[i] - 1];
    }
    string left = perm.substr(0, 32);
    string right = perm.substr(32, 32);

    for(int i = 0; i<16; i++){
        string right_expanded = "";
        for(int j = 0; j<48; j++){
            right_expanded += right[expansion_table[j] - 1];
        }
        string xored = Xor(round_keys[i], right_expanded);
        string res = "";
        for(int j = 0; j<8; j++){
            string row1 = xored.substr(j*6,1) + xored.substr(j*6 + 5,1);
            int row = convertBinaryToDecimal(row1);
            string col = xored.substr(j*6 + 1,1) + xored.substr(j*6 + 2,1) + xored.substr(j*6 + 3,1) + xored.substr(j*6 + 4,1);
            int col_int = convertBinaryToDecimal(col);
            int val = substitution_boxes[j][row][col_int];
            res += convertDecimalToBinary(val);
        }
        string perm2 = "";
        for(int k = 0; k<32; k++){
            perm2 += res[permutation_tab[k]-1];
        }
        xored = Xor(perm2, left);
        left = xored;
        if(i<15){
            string temp = right;
            right = xored;
            left = temp;
        }
    }
    string combined_text = left + right;
    string ciphertext = "";
    for(int i = 0; i<64; i++){
        ciphertext += combined_text[inverse_permutation[i]-1];
    }
    return ciphertext;
}

int main(){
    cout<<"Name: Khushbu Jain"<<endl;
    cout<<"Roll Number : 23115047"<<endl;
    string key, apt;
    cout << "Enter key: ";
    getline(cin, key);
    cout << "Enter plain text: ";
    getline(cin, pt);
    apt = pt;
    pt = textToBinary(pt);
    key = textToBinary(key);
    generate_keys(key);
    cout<<"Plain text: "<<apt<<endl;
    string ct = DES();
    cout<<"Ciphertext: "<<ct<<endl;
    // Reverse round keys for decryption
    int i = 15;
    int j = 0;
    while(i > j){
        string temp = round_keys[i];
        round_keys[i] = round_keys[j];
        round_keys[j] = temp;
        i--;
        j++;
    }
    pt = ct;
    string decrypted_binary = DES();
    string decrypted_text = binaryToText(decrypted_binary);
    cout << "Decrypted text: " << decrypted_text << endl;
    if (decrypted_text == apt){
        cout<<"Plain text encrypted and decrypted successfully."<<endl;
    }
    return 0;
}
/*
Output:
Enter key: 23115047
Enter plain text: Khushbu
Plain text: Khushbu
Ciphertext: 1010000011000110100010011100000000001000101000001111000000100100
Decrypted text: Khushbu
Plain text encrypted and decrypted successfully.
*/