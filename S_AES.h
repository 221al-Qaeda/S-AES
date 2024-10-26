//
// Created by 王镇东 on 2024/10/22.
//

#ifndef HOMEWORK2_S_AES_H
#define HOMEWORK2_S_AES_H

#include <iostream>
#include <vector>
using namespace std;

class S_AES {
private:
    // 半字节替代表
    const  vector< vector<int>> sbox = {
            {0x9, 0x4, 0xA, 0xB},
            {0xD, 0x1, 0x8, 0x5},
            {0x6, 0x2, 0x0, 0x3},
            {0xC, 0xE, 0xF, 0x7}
    };
    // 逆半字节替代表
    const vector<vector<int>> invSbox = {
            {0xA, 0x5, 0x9, 0xB},
            {0x1, 0x7, 0x8, 0xF},
            {0x6, 0x0, 0x2, 0x3},
            {0xC, 0x4, 0xD, 0xE}
    };
    // 轮常数
    const vector<int>rocn1 = { 1,0,0,0,0,0,0,0 };
    const vector<int>rocn2 = { 0,0,1,1,0,0,0,0 };
private:
    vector<int> substituteNibble(vector<int> state);                    // 替换函数
    vector<int> inverseSubstituteNibble(vector<int>state);              // 逆半字节替代函数
    vector<int> shiftRows(vector<int>& state);                          // 行移位函数
    vector<int> inverseShiftRows(vector<int>& state);                   // 逆行移位函数
    void mixColumns(vector<int>& state);                                // 列混淆函数
    void inverseMixColumns(vector<int>& state);                         // 逆列混淆函数
    vector<int> RotNib(vector<int> state);                              // 交换前后
    vector<vector<int>> keyExpansion(vector<int> key);                  // 密钥拓展函数
public:
    friend vector<int> hexToBinary(int hexValue);                       // 十六进制数转换为二进制数组
    friend int binaryVectorToDecimal(const vector<int>& binary);        // 二进制数组转换成十六进制
    friend void printVector(const vector<int>& vec);                    // 输出vector
public:
    vector<int> ASCIIEncrypt(string plaintext, vector<int> key);        // ASCII编码字符串加密函数
    vector<int> encrypt(vector<int> plaintext, vector<int> key);        // 加密函数
    vector<int> decrypt(vector<int> ciphertext, vector<int> key);       // 解密函数
    vector<int> doubleEncrypt(vector<int> plaintext, vector<int> key);  // 双重加密
    vector<int> doubleDecrypt(vector<int> ciphertext, vector<int> key); // 双重解密
    vector<int> tripleEncrypt(vector<int> plaintext, vector<int> key);  // 三重加密
    vector<int> tripleDecrypt(vector<int> ciphertext, vector<int> key); // 三重解密
};

vector<int> hexToBinary(int hexValue) {
    vector<int> binary;
    int temp = hexValue;
    while (temp > 0) {
        binary.push_back(temp % 2);
        temp /= 2;
    }
    while (binary.size() < 4) {
        binary.push_back(0);
    }
    reverse(binary.begin(), binary.end());
    return binary;
}

int binaryVectorToDecimal(const vector<int>& binary) {
    int decimal = 0;
    int n = binary.size();
    for (int i = 0; i < n; ++i) {
        decimal += pow(2, n - 1 - i) * binary[i];
    }
    return decimal;
}

void printVector(const vector<int>& vec) {
    cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        cout << vec[i];
        if (i < vec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

#endif //HOMEWORK2_S_AES_H















