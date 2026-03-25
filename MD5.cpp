#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
using namespace std;

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define LEFT_ROTATE(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

const uint32_t K[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

const uint32_t S[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

string md5(string msg) {
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xefcdab89;
    uint32_t h2 = 0x98badcfe;
    uint32_t h3 = 0x10325476;

    uint64_t msg_len = msg.length();
    
    // Padding
    msg += (char)0x80;
    while (msg.length() % 64 != 56) {
        msg += (char)0;
    }
    
    // Append length
    uint64_t bit_len = msg_len * 8;
    for (int i = 0; i < 8; i++) {
        msg += (char)((bit_len >> (i * 8)) & 0xFF);
    }
    
    // Process each 512-bit chunk
    for (size_t i = 0; i < msg.length(); i += 64) {
        uint32_t w[16];
        
        // Break chunk into 16 32-bit words
        for (int j = 0; j < 16; j++) {
            w[j] = (uint32_t)((unsigned char)msg[i + j * 4]) |
                   ((uint32_t)((unsigned char)msg[i + j * 4 + 1]) << 8) |
                   ((uint32_t)((unsigned char)msg[i + j * 4 + 2]) << 16) |
                   ((uint32_t)((unsigned char)msg[i + j * 4 + 3]) << 24);
        }
        
        uint32_t a = h0, b = h1, c = h2, d = h3;
        
        // Main loop
        for (int j = 0; j < 64; j++) {
            uint32_t f, g;
            
            if (j < 16) {
                f = F(b, c, d);
                g = j;
            } else if (j < 32) {
                f = G(b, c, d);
                g = (5 * j + 1) % 16;
            } else if (j < 48) {
                f = H(b, c, d);
                g = (3 * j + 5) % 16;
            } else {
                f = I(b, c, d);
                g = (7 * j) % 16;
            }
            
            uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFT_ROTATE((a + f + K[j] + w[g]), S[j]);
            a = temp;
        }
        
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }
    
    // Convert to hex string
    stringstream ss;
    uint32_t results[] = {h0, h1, h2, h3};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ss << hex << setw(2) << setfill('0') << ((results[i] >> (j * 8)) & 0xFF);
        }
    }
    
    return ss.str();
}

int main() {
    cout<<"Name: Khushbu Jain"<<endl;
    cout<<"Roll Number: 23115047"<<endl;
    
    string input;
    cout << "Enter string to hash: ";
    getline(cin, input);
    cout << "MD5: " << md5(input) << endl;
    return 0;
}
/*
Enter string to hash: Khushbu Jain
MD5: 4236d7dba6cf8be9f2e70a79eece9851
*/