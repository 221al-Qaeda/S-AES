#include<iostream>
#include <thread>
#include<mutex>
#include<vector>
#include<unordered_map>
#include"S_AES.h"
using namespace std;
S_AES s_aes;
mutex mtx1, mtx2;
int cnt = 0;
void getMidTexts(vector<int> plaintext, int start, int end, unordered_map<int, int>& mid_texts) {
    for (int i = start; i < end; ++i) {
        vector<int> K1 = hexToBinary(i, 16);
        lock_guard<mutex> lock(mtx1);
        mid_texts[binaryVectorToDecimal(s_aes.encrypt(plaintext, K1))] = i;
    }
}
void getPossibleKeys(vector<int> ciphertext, int start, int end, const unordered_map<int, int>& mid_texts) {
    for (int i = start; i < end; ++i) {
        vector<int> K2 = hexToBinary(i, 16);
        if (mid_texts.find(binaryVectorToDecimal(s_aes.decrypt(ciphertext, K2))) != mid_texts.end()) {
            vector<int> K1 = hexToBinary(mid_texts.find(binaryVectorToDecimal(s_aes.decrypt(ciphertext, K2)))->second, 16);
            lock_guard<mutex> lock(mtx2);
            std::cout << "���ܵ���ԿΪ��";
            std::cout << "[";
            for (int i = 0; i < 16; ++i) {
                std::cout << K1[i] << ",";
            }
            for (int i = 0; i < 16; ++i) {
                std::cout << K2[i];
                if (i < 15) {
                    std::cout << ",";
                }
            }
            std::cout << "]" << endl;
            ++cnt;
        }
    }
}
int main() {
    vector<int> plaintext = { 1,0,1,0,0,1,1,1,0,1,0,0,1,0,0,1 };
    vector<int> key = {1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1};
    vector<int> doubleKey = { 0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0 };
    vector<int> tripleKey = { 1,0,1,0,0,1,1,1,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,1 };
    /*cout << "����Ϊ��";
    string plaintext = "plaintext";
    cout<<plaintext<<endl;
    string ciphertext = s_aes.ASCIIEncrypt(plaintext, key);
    cout << "��ASCII���ܺ������Ϊ��" << ciphertext << endl;*/
    cout << "����Ϊ��";
    printVector(plaintext);
    cout << "˫�ؼ��ܺ�����Ϊ��";
    vector<int> ciphertext = s_aes.doubleEncrypt(plaintext, doubleKey);
    printVector(ciphertext);
    vector<int> newPlaintext = s_aes.doubleDecrypt(ciphertext, doubleKey);
    cout << "�����Ľ��ܺ�õ������ģ�";
    printVector(newPlaintext);
    unordered_map<int, int> mid_texts;
    vector<thread> thread_pool;
    for (int i = 0; i < 8; ++i) {
        thread_pool.push_back(thread(getMidTexts, plaintext, i * 8192, (i + 1) * 8192, ref(mid_texts)));
    }
    
    for (int i = 0; i < 8; ++i) {
        thread_pool[i].join();
    }
    thread_pool.clear();
    for (int i = 0; i < 8; ++i) {
        thread_pool.push_back(thread(getPossibleKeys, ciphertext, i * 8192, (i + 1) * 8192, ref(mid_texts)));
    }
    for (int i = 0; i < 8; ++i) {
        thread_pool[i].join();
    }
    thread_pool.clear();
    cout << "���ܵ���Կ�ܹ���" << cnt << "��" << endl;
    cout << "���ؼ��ܺ�����Ϊ��";
    ciphertext = s_aes.tripleEncrypt(plaintext, tripleKey);
    printVector(ciphertext);
    newPlaintext = s_aes.tripleDecrypt(ciphertext, tripleKey);
    cout << "�����Ľ��ܺ�õ������ģ�";
    printVector(newPlaintext);
}