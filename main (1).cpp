#include<iostream>
#include<vector>
#include"S_AES.h"
using namespace std;
S_AES s_aes;
int main() {
    vector<int> plaintext = {1,0,1,0,0,1,1,1,0,1,0,0,1,0,0,1 };
    vector<int> doubleKey = {0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0};
    vector<int> tripleKey = {1,0,1,0,0,1,1,1,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,1};
    cout << "明文为：";
    printVector(plaintext);
    cout<<"双重加密后密文为：";
    vector<int>ciphertext = doubleEncrypt(plaintext, doubleKey);
    printVector(ciphertext);
    vector<int> newPlaintext = doubleDecrypt(ciphertext, doubleKey);
    cout << "经密文解密后得到的明文：";
    printVector(newPlaintext);
    for (int i = 0; i < 65536; ++i) {
        vector<int> K1 = hexToBinary(i);
        for (int j = 0; j < 65536; ++j) {
            vector<int> K2 = hexTobinary(j);
            if (binaryVectorToDecimal(s_aes.encrypt(plaintext, K1)) == binaryVectorToDecimal(s_aes.decrypt(ciphertext, K2))) {
                cout<<"可能的密钥为：";
                cout << "[";
                for (int i = 0; i < 16; ++i) {
                    cout<<K1[i]<<",";
                }
                for (int i = 0; i < 16; ++i) {
                    cout<<K2[i];
                    if (i < 15) {
                        cout<<",";
                    }
                }
                cout << "]" << endl;
            }
        }
    }
    cout<<"三重加密后密文为：";
    ciphertext = tripleEncrypt(plaintext, tripleKey);
    printVector(ciphertext);
    newPlaintext = tripleDecrypt(ciphertext, tripleKey);
    cout << "经密文解密后得到的明文：";
    printVector(newPlaintext);

}