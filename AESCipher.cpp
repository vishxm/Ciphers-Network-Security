/*
    Advanced Encryption Standard Implementation
    Made by UI17CO49

    - alias for bitsets with 8 and 32 size
    - GFMul galois field multiplication 
    - 4 step subbytes, shiftrows, mixcolumns, addroundkey
*/
#include <iostream>  
#include <bitset>  
#include <string>  
using namespace std;   

typedef bitset<8> byte;  
typedef bitset<32> word;  
  
// lookup table  
byte S_Box[16][16] = {  
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},  
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},  
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},  
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},  
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},  
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},  
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},  
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},  
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},  
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},  
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},  
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},  
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},  
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},  
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},  
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}  
};   
  
word Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,   
                 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};  
   
byte GFMul(byte a, byte b) {   //galois field multiplication
    byte p = 0;  
    byte hbs;  
    for (int counter = 0; counter < 8; counter++) {  
        if ((b & byte(1)) != 0) {  
            p ^= a;  
        }  
        hbs = (byte) (a & byte(0x80));  
        a <<= 1;  
        if (hbs != 0) {  
            a ^= 0x1b;   
        }  
        b >>= 1;  
    }  
    return p;  
}

// 1st STEP    
void SubBytes(byte mtx[4*4])  
{  
    for(int i=0; i<16; ++i)  
    {  
        int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];  
        int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];  
        mtx[i] = S_Box[row][col];  
    }  
}  
   
// 2nd STEP   
void ShiftRows(byte mtx[4*4])  
{  
    byte temp = mtx[4]; //2nd row  
    for(int i=0; i<3; ++i)  
        mtx[i+4] = mtx[i+5];  
    mtx[7] = temp;  
    
    for(int i=0; i<2; ++i)  //3rd row
    {  
        temp = mtx[i+8];  
        mtx[i+8] = mtx[i+10];  
        mtx[i+10] = temp;  
    }  
    
    temp = mtx[15];  
    for(int i=3; i>0; --i)  //4th row
        mtx[i+12] = mtx[i+11];  
    mtx[12] = temp;  
}  
      
// 3rd STEP 
void MixColumns(byte mtx[4*4])  
{  
    byte arr[4];  
    for(int i=0; i<4; ++i)  
    {  
        for(int j=0; j<4; ++j)  
            arr[j] = mtx[i+j*4];  
  
        mtx[i] = GFMul(0x02, arr[0]) ^ GFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];  
        mtx[i+4] = arr[0] ^ GFMul(0x02, arr[1]) ^ GFMul(0x03, arr[2]) ^ arr[3];  
        mtx[i+8] = arr[0] ^ arr[1] ^ GFMul(0x02, arr[2]) ^ GFMul(0x03, arr[3]);  
        mtx[i+12] = GFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GFMul(0x02, arr[3]);  
    }  
}  

// 4th STEP
void AddRoundKey(byte mtx[4*4], word k[4])  
{  
    for(int i=0; i<4; ++i)  
    {  
        word k1 = k[i] >> 24;  
        word k2 = (k[i] << 8) >> 24;  
        word k3 = (k[i] << 16) >> 24;  
        word k4 = (k[i] << 24) >> 24;  
          
        mtx[i] = mtx[i] ^ byte(k1.to_ulong());  
        mtx[i+4] = mtx[i+4] ^ byte(k2.to_ulong());  
        mtx[i+8] = mtx[i+8] ^ byte(k3.to_ulong());  
        mtx[i+12] = mtx[i+12] ^ byte(k4.to_ulong());  
    }  
}  

word Word(byte& k1, byte& k2, byte& k3, byte& k4)  
{  
    word result(0x00000000);  
    word temp;  
    temp = k1.to_ulong();  // K1  
    temp <<= 24;  
    result |= temp;  
    temp = k2.to_ulong();  // K2  
    temp <<= 16;  
    result |= temp;  
    temp = k3.to_ulong();  // K3  
    temp <<= 8;  
    result |= temp;  
    temp = k4.to_ulong();  // K4  
    result |= temp;  
    return result;  
}  

word RotWord(word& rw)  
{  
    word high = rw << 8;  
    word low = rw >> 24;  
    return high | low;  
}  
  
word SubWord(const word& sw)  
{  
    word temp;  
    for(int i=0; i<32; i+=8)  
    {  
        int row = sw[i+7]*8 + sw[i+6]*4 + sw[i+5]*2 + sw[i+4];  
        int col = sw[i+3]*8 + sw[i+2]*4 + sw[i+1]*2 + sw[i];  
        byte val = S_Box[row][col];  
        for(int j=0; j<8; ++j)  
            temp[i+j] = val[j];  
    }  
    return temp;  
}  
  
void KeyExpansion(byte key[16], word w[8])  
{  
    word temp;  
    int i = 0;  
    while(i < 4)   
    {  
        w[i] = Word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);  
        ++i;  
    }  
  
    i = 4;  
    while(i < 8 )
    {  
        temp = w[i-1]; 
        if(i % 4 == 0)  
            w[i] = w[i-4] ^ SubWord(RotWord(temp)) ^ Rcon[i/4-1];  
        else   
            w[i] = w[i-4] ^ temp;  
        ++i;  
    }  
}  
  

void encrypt(byte in[16], word w[8])  
{  
    word key[4];
    for(int i=0; i<4; ++i)  
        key[i] = w[i];  
    AddRoundKey(in, key); 

    cout<<"\nPre-round Transformation : \n\n";
    cout<<"\t";
    for(int i=0; i<16; ++i)  
    {  
        cout << hex << in[i].to_ulong() << " ";  
        if((i+1)%4 == 0)  
            cout << endl << "\t";  
    }  
    cout << endl;   
  
      
    SubBytes(in);  
    ShiftRows(in);  
    MixColumns(in);  
    for(int i=0; i<4; ++i)  
        key[i] = w[4+i];  
    AddRoundKey(in, key);  
      
    cout<<"\nTransformation in function : \n\n";
    cout<<"\t";
    for(int i=0; i<16; ++i)  
    {  
        cout << hex << in[i].to_ulong() << " ";  
        if((i+1)%4 == 0)  
            cout << endl << "\t";  
    }  
    cout << endl;   
}
   

int main()  
{  
    cout << "\n------------------------------------------------------------------------------";
    cout << "\n\n\t\tADVANCED ENCRYPTION STANDARD IMPLEMENTATION ";
    cout << "\n------------------------------------------------------------------------------\n\n";
    byte key[16] = {0x4B, 0x4D, 0xBC, 0x09,
                    0x34, 0x92, 0x35, 0xE4,
                    0xE8, 0x67, 0x9A, 0x0F,
                    0xF8, 0x9F, 0xF2, 0x73};  
  
    byte plain[16] = {0x34, 0x9A, 0x35, 0xE4,
                      0x43, 0x5B, 0x31, 0x37,
                      0xF8, 0x34, 0x9B, 0x0A,
                      0xA9, 0x8E, 0xA2, 0x34};
    
    cout << "\n------------------------------------------------------------------------------\n\n"; 
    cout << "\n\tKey : ";  
    for(int i=0; i<16; ++i)  
        cout << hex << key[i].to_ulong() << " ";  
    cout << endl;
    cout << "\n------------------------------------------------------------------------------\n\n";  
  
    word w[8];  
    KeyExpansion(key, w);  
  
    cout << "\n------------------------------------------------------------------------------\n\n";
    cout << endl << "Plaintext :\n"<<endl;  
    cout<<"\t";
    for(int i=0; i<16; ++i)  
    {  
        cout << hex << plain[i].to_ulong() << " ";  
        if((i+1)%4 == 0)  
            cout << endl << "\t";  
    }  
    cout << endl; 
    cout << "\n------------------------------------------------------------------------------\n\n"; 
  
    encrypt(plain, w);  
    cout << "\n------------------------------------------------------------------------------";
    cout << "\n\nCiphertext :\n"<<endl;  
    cout << "\t";
    for(int i=0; i<16; ++i)  
    {  
        cout << hex << plain[i].to_ulong() << " ";  
        if((i+1)%4 == 0)  
            cout << endl << "\t";  
    }  
    cout << "\n------------------------------------------------------------------------------\n\n"; 

    cout<<"\n\n\t\t:)\n\n\n\n";
    return 0;  
}  
    
