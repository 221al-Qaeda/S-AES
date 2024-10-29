//
// Created by ���� on 2024/10/22.
//

#ifndef HOMEWORK2_S_AES_H
#define HOMEWORK2_S_AES_H

#include <iostream>
#include <vector>
using namespace std;

class S_AES {
private:
    // ���ֽ������
    const  vector< vector<int>> sbox = {
            {0x9, 0x4, 0xA, 0xB},
            {0xD, 0x1, 0x8, 0x5},
            {0x6, 0x2, 0x0, 0x3},
            {0xC, 0xE, 0xF, 0x7}
    };
    // ����ֽ������
    const vector<vector<int>> invSbox = {
            {0xA, 0x5, 0x9, 0xB},
            {0x1, 0x7, 0x8, 0xF},
            {0x6, 0x0, 0x2, 0x3},
            {0xC, 0x4, 0xD, 0xE}
    };
    // �ֳ���
    const vector<int>rocn1 = { 1,0,0,0,0,0,0,0 };
    const vector<int>rocn2 = { 0,0,1,1,0,0,0,0 };
private:
    vector<int> substituteNibble(vector<int> state);                    // �滻����
    vector<int> inverseSubstituteNibble(vector<int>state);              // ����ֽ��������
    vector<int> shiftRows(vector<int>& state);                          // ����λ����
    vector<int> inverseShiftRows(vector<int>& state);                   // ������λ����
    void mixColumns(vector<int>& state);                                // �л�������
    void inverseMixColumns(vector<int>& state);                         // ���л�������
    vector<int> RotNib(vector<int> state);                              // ����ǰ��
    vector<vector<int>> keyExpansion(vector<int> key);                  // ��Կ��չ����
public:
    friend vector<int> hexToBinary(int hexValue, int size);                       // ʮ��������ת��Ϊ����������
    friend int binaryVectorToDecimal(const vector<int>& binary);        // ����������ת����ʮ������
    friend void printVector(const vector<int>& vec);                    // ���vector
public:
    string ASCIIEncrypt(string plaintext, vector<int> key);        // ASCII�����ַ������ܺ���
    vector<int> encrypt(vector<int> plaintext, vector<int> key);        // ���ܺ���
    vector<int> decrypt(vector<int> ciphertext, vector<int> key);       // ���ܺ���
    vector<int> doubleEncrypt(vector<int> plaintext, vector<int> key);  // ˫�ؼ���
    vector<int> doubleDecrypt(vector<int> ciphertext, vector<int> key); // ˫�ؽ���
    vector<int> tripleEncrypt(vector<int> plaintext, vector<int> key);  // ���ؼ���
    vector<int> tripleDecrypt(vector<int> ciphertext, vector<int> key); // ���ؽ���
};

inline vector<int> hexToBinary(int hexValue, int size) {
    vector<int> binary;
    int temp = hexValue;
    while (temp > 0) {
        binary.push_back(temp % 2);
        temp /= 2;
    }
    while (binary.size() < size) {
        binary.push_back(0);
    }
    reverse(binary.begin(), binary.end());
    return binary;
}

inline int binaryVectorToDecimal(const vector<int>& binary) {
    int decimal = 0;
    int n = binary.size();
    for (int i = 0; i < n; ++i) {
        decimal += pow(2, n - 1 - i) * binary[i];
    }
    return decimal;
}

inline void printVector(const vector<int>& vec) {
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