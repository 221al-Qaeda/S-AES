//
// Created by promise on 2024/10/26.
//

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "S_AES.h"

using namespace std;
S_AES s_aes;

//生成初始变量IV
vector<int> generateIV() {
    vector<int> IV;
    random_device randomDevice; //获取随机数种子
    mt19937  gen(randomDevice());
    uniform_int_distribution<> dis(0,1);

    for (int i = 0; i < 16; ++i) {
        IV.push_back(dis(gen));
    }
    return IV;
}


//将输入的字符串转换为vector
vector<int> binaryStringToVector(const string& binaryString) {
    vector<int> result;
    for(char ch: binaryString) {
        if (ch == '0') {
            result.push_back(0);
        } else if (ch == '1') {
            result.push_back(1);
        }
    }
    return result;
}

//分组
vector<vector<int>> splitIntoGroup(const vector<int>& input) {
    vector<vector<int>> result;
    const int groupSize = 16;
    int size = input.size();
    for (int i = 0; i < size; i += groupSize) {
        vector<int> group(input.begin() + i, input.begin() + (min(size, i + groupSize)));

        //判断分组长度是否为16，不是就补0
        while (group.size() < 16)
        {
            group.push_back(0);
        }
        result.push_back(group);
    }
    return result;
}

//异或
vector<int> xorVector(const vector<int>& vec1, const vector<int>& vec2) {
    vector<int> result(vec1.size());
    for (int i = 0; i < vec1.size(); ++i) {
        result[i] = vec1[i] xor vec2[i];
    }
    return result;
}

vector<vector<int>> CBCEncrypt(const string& plainText, const vector<int>& key, const vector<int>& IV) {
    vector<int> plainTextVector = binaryStringToVector(plainText);
    vector<vector<int>> plainTextGroups = splitIntoGroup(plainTextVector);



    vector<vector<int>> cipherText;
    vector<int> temp = xorVector(IV,plainTextGroups[0]);
    vector<int> cipherTextGroup = s_aes.encrypt(temp, key);
    cipherText.push_back(cipherTextGroup);
    for(vector<vector<int>>::iterator it = plainTextGroups.begin() + 1; it != plainTextGroups.end(); it++) {
        temp.clear();
        temp = xorVector(cipherTextGroup, *it);
        cipherTextGroup.clear();
        cipherTextGroup = s_aes.encrypt(temp, key);
        cipherText.push_back(cipherTextGroup);
    }
    return cipherText;
}

vector<vector<int>> CBCDecrypt( vector<vector<int>>& cipherTextGroups, const vector<int>& key, const vector<int>& IV) {
    vector<vector<int>> plainText;
    vector<int> plainTextGroup = s_aes.decrypt(cipherTextGroups[0], key);
    vector<int> temp = xorVector(IV, plainTextGroup);
    plainText.push_back(temp);
    for (size_t i = 1; i < cipherTextGroups.size(); ++i) {
        // 解密当前密文组
        plainTextGroup = s_aes.decrypt(cipherTextGroups[i], key);
        // 使用前一个密文组进行异或计算明文组
        vector<int> temp = xorVector(cipherTextGroups[i - 1], plainTextGroup);
        plainText.push_back(temp);
    }
    return plainText;
}

int main() {
    //vector<int> IV = generateIV();
    //cout << "使用的初始向量为：";
    //printVector(IV);

    //string plainText;
    //cin >> plainText;

    //cout << "明文为：" << " " << plainText << endl;
    vector<int> key = { 0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1 };

    //vector<vector<int>> result = CBCEncrypt(plainText, key, IV);
    //cout << "明文分组为：" << endl;
    //for(vector<vector<int>>::iterator it = result.begin(); it != result.end(); it++) {
    //    printVector(*it);
    //}

    //vector<vector<int>> cipherTextGroups = { {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1},
    //    {0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1},
    //    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1} };
    //vector<vector<int>> result = CBCDecrypt(cipherTextGroups, key, IV);
    //cout << "密文分组为：" << endl;
    //for(vector<vector<int>>::iterator it = result.begin(); it != result.end(); it++) {
    //    printVector(*it);
    //}

    return 0;
}