//
// Created by 王镇东 on 2024/10/22.
//

#include "S_AES.h"

vector<int> S_AES::substituteNibble(vector<int> state) {
    // 检查 state 的长度是否是4的倍数
    if (state.size() % 4 != 0) {
        cerr << "State size must be a multiple of 4." << endl;
        return {};
    }

    vector<int> result;
    result.reserve(state.size());

    // 按每四个位分为一组进行替换
    for (size_t i = 0; i < state.size(); i += 4) {
        int row1 = state[i] * 2 + state[i + 1];
        int col1 = state[i + 2] * 2 + state[i + 3];
        vector<int> nibble = hexToBinary(sbox[row1][col1], 4);
        result.insert(result.end(), nibble.begin(), nibble.end());
    }

    return result;
}

vector<int> S_AES::inverseSubstituteNibble(vector<int>state) {
    // 检查 state 的长度是否是4的倍数
    if (state.size() % 4 != 0) {
        cerr << "State size must be a multiple of 4." << endl;
        return {};
    }

    vector<int> result;
    result.reserve(state.size());

    // 按每四个位分为一组进行替换
    for (size_t i = 0; i < state.size(); i += 4) {
        int row1 = state[i] * 2 + state[i + 1];
        int col1 = state[i + 2] * 2 + state[i + 3];
        vector<int> nibble = hexToBinary(invSbox[row1][col1], 4);
        result.insert(result.end(), nibble.begin(), nibble.end());
    }

    return result;
}

vector<int> S_AES::shiftRows(vector<int>& state) {
    int temp1 = 0, temp2 = 0;
    for (int i = 4; i <= 7; i++) {
        temp1 |= state[i] << (i - 4);
    }
    for (int i = 12; i <= 15; i++) {
        temp2 |= state[i] << (i - 12);
    }
    for (int i = 4; i <= 7; i++) {
        state[i] = (temp2 >> (i - 4)) & 1;
    }
    for (int i = 12; i <= 15; i++) {
        state[i] = (temp1 >> (i - 12)) & 1;
    }
    return state;
}

vector<int> S_AES::inverseShiftRows(vector<int>& state) {
    int temp1 = 0, temp2 = 0;
    for (int i = 4; i <= 7; i++) {
        temp1 |= state[i] << (i - 4);
    }
    for (int i = 12; i <= 15; i++) {
        temp2 |= state[i] << (i - 12);
    }
    for (int i = 4; i <= 7; i++) {
        state[i] = (temp2 >> (i - 4)) & 1;
    }
    for (int i = 12; i <= 15; i++) {
        state[i] = (temp1 >> (i - 12)) & 1;
    }
    return state;
}

void S_AES::inverseMixColumns(vector<int>& state) {
    vector<int> newState = state;
    state[0] = newState[3] ^ newState[5];
    state[1] = newState[0] ^ newState[6];
    state[2] = newState[1] ^ newState[4] ^ newState[7];
    state[3] = newState[2] ^ newState[4] ^ newState[3];
    state[4] = newState[1] ^ newState[7];
    state[5] = newState[2] ^ newState[4];
    state[6] = newState[0] ^ newState[5] ^ newState[3];
    state[7] = newState[0] ^ newState[7] ^ newState[6];
    state[8] = newState[11] ^ newState[13];
    state[9] = newState[8] ^ newState[14];
    state[10] = newState[9] ^ newState[12] ^ newState[15];
    state[11] = newState[10] ^ newState[12] ^ newState[11];
    state[12] = newState[9] ^ newState[15];
    state[13] = newState[10] ^ newState[12];
    state[14] = newState[8] ^ newState[13] ^ newState[11];
    state[15] = newState[8] ^ newState[15] ^ newState[14];
}

void S_AES::mixColumns(vector<int>& state) {
    vector<int> newState = state;
    state[0] = newState[0] ^ newState[6];
    state[1] = newState[1] ^ newState[4] ^ newState[7];
    state[2] = newState[2] ^ newState[4] ^ newState[5];
    state[3] = newState[3] ^ newState[5];
    state[4] = newState[2] ^ newState[4];
    state[5] = newState[0] ^ newState[3] ^ newState[5];
    state[6] = newState[0] ^ newState[1] ^ newState[6];
    state[7] = newState[1] ^ newState[7];
    state[8] = newState[8] ^ newState[14];
    state[9] = newState[9] ^ newState[12] ^ newState[15];
    state[10] = newState[10] ^ newState[12] ^ newState[13];
    state[11] = newState[11] ^ newState[13];
    state[12] = newState[10] ^ newState[12];
    state[13] = newState[8] ^ newState[11] ^ newState[13];
    state[14] = newState[8] ^ newState[9] ^ newState[14];
    state[15] = newState[9] ^ newState[15];
}

vector<int> S_AES::RotNib(vector<int> state) {
    vector<int>temp(4);
    for (int i = 0; i < 4; i++) {
        temp[i] = state[i];
    }
    for (int i = 0; i < 4; i++) {
        state[i] = state[i + 4];
    }
    for (int i = 0; i < 4; i++) {
        state[i + 4] = temp[i];
    }
    return state;
}

vector<vector<int>> S_AES::keyExpansion(vector<int> key) {
    vector<vector<int>> w(6, vector<int>(8, 0));
    //将输入 key 的前8位存储在 w[0] 中
    for (int i = 0; i < 8; i++) {
        w[0][i] = key[i];
    }

    // 将输入 key 的后8位存储在 w[1] 中
    for (int i = 8; i < 16; ++i) {
        w[1][i - 8] = key[i];
    }
    vector<int>temp1 = RotNib(w[1]);
    temp1 = substituteNibble(temp1);
    for (int i = 0; i < 8; i++) {
        w[2][i] = w[0][i] ^ rocn1[i] ^ temp1[i];
    }
    for (int i = 0; i < 8; i++) {
        w[3][i] = w[2][i] ^ w[1][i];
    }
    vector<int>temp2 = RotNib(w[3]);
    temp2 = substituteNibble(temp2);
    for (int i = 0; i < 8; i++) {
        w[4][i] = w[2][i] ^ rocn2[i] ^ temp2[i];
    }
    for (int i = 0; i < 8; i++) {
        w[5][i] = w[4][i] ^ w[3][i];
    }
    vector<int>key0(16);
    vector<int>key1(16);
    vector<int>key2(16);
    for (int i = 0; i < 8; i++) {
        key0[i] = w[0][i];
        key0[i + 8] = w[1][i];
    }
    for (int i = 0; i < 8; i++) {
        key1[i] = w[2][i];
        key1[i + 8] = w[3][i];
    }
    for (int i = 0; i < 8; i++) {
        key2[i] = w[4][i];
        key2[i + 8] = w[5][i];
    }
    vector<vector<int>>keyfinal(3, vector<int>(16, 0));
    for (int i = 0; i < 16; i++) {
        keyfinal[0][i] = key0[i];
        keyfinal[1][i] = key1[i];
        keyfinal[2][i] = key2[i];
    }
    return keyfinal;
}

vector<int> S_AES::encrypt(vector<int> plaintext, vector<int> key) {
    vector<int> state = plaintext;
    vector<vector<int>>Key;

    Key = keyExpansion(key);
    // 初始轮密钥加
    //cout << "密钥0：";
    //printVector(Key[0]);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[0][i];
    }
    //cout << "初始轮密钥加:";
    //printVector(state);
    // 第一轮
    state = substituteNibble(state);
    //cout << "半字节替换：";
    //printVector(state);
    shiftRows(state);
    //cout << "行变化：";
    //printVector(state);
    mixColumns(state);
    //cout << "列混淆：";
    //printVector(state);
    //cout << "密钥1：";
    //printVector(Key[1]);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[1][i];
    }
    //cout << "第一论轮密钥加:";
    //printVector(state);
    // 第二轮

    state = substituteNibble(state);
    //cout << "半字节替换：";
    //printVector(state);
    shiftRows(state);
    //cout << "行变化：";
    //printVector(state);
    //cout << "密钥2：";
    //printVector(Key[2]);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[2][i];
    }
    //cout << "第二论轮密钥加:";
    //printVector(state);
    return state;
}

vector<int> S_AES::decrypt(vector<int> ciphertext, vector<int> key) {
    vector<int> state = ciphertext;
    vector<vector<int>>Key;
    //cout << "密文是："<<endl;
    //printVector(ciphertext);
    Key = keyExpansion(key);
    // 初始轮密钥加
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[2][i];
    }
    //cout << "初始轮密钥加:";
    //printVector(state);
    // 第一轮逆
    inverseShiftRows(state);
    //cout << "行变化：";
    //printVector(state);
    state = inverseSubstituteNibble(state);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[1][i];
    }
    inverseMixColumns(state);

    // 第二轮逆
    inverseShiftRows(state);
    state = inverseSubstituteNibble(state);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[0][i];
    }

    return state;
}

vector<int> S_AES::doubleEncrypt(vector<int> plaintext, vector<int> key) {
    vector<int> K1;
    vector<int> K2;
    for (int i = 0; i < 16; ++i) {
        K1.push_back(key[i]);
    }
    for (int i = 16; i < 32; ++i) {
        K2.push_back(key[i]);
    }
    return encrypt(encrypt(plaintext, K1), K2);
}

vector<int> S_AES::doubleDecrypt(vector<int> ciphertext, vector<int> key) {
    vector<int> K1;
    vector<int> K2;
    for (int i = 0; i < 16; ++i) {
        K1.push_back(key[i]);
    }
    for (int i = 16; i < 32; ++i) {
        K2.push_back(key[i]);
    }
    return decrypt(decrypt(ciphertext, K2), K1);
}

vector<int> S_AES::tripleEncrypt(vector<int> plaintext, vector<int> key) {
    vector<int> K1;
    vector<int> K2;
    vector<int> K3;
    for (int i = 0; i < 16; ++i) {
        K1.push_back(key[i]);
    }
    for (int i = 16; i < 32; ++i) {
        K2.push_back(key[i]);
    }
    for (int i = 32; i < 48; ++i) {
        K3.push_back(key[i]);
    }
    return encrypt(encrypt(encrypt(plaintext, K1), K2), K3);
}

vector<int> S_AES::tripleDecrypt(vector<int> ciphertext, vector<int> key) {
    vector<int> K1;
    vector<int> K2;
    vector<int> K3;
    for (int i = 0; i < 16; ++i) {
        K1.push_back(key[i]);
    }
    for (int i = 16; i < 32; ++i) {
        K2.push_back(key[i]);
    }
    for (int i = 32; i < 48; ++i) {
        K3.push_back(key[i]);
    }
    return decrypt(decrypt(decrypt(ciphertext, K3), K2), K1);
}

string S_AES::ASCIIEncrypt(string plaintext, vector<int> key) {
    string ciphertext = "";
    for (int i = 0; i < plaintext.size(); ++i) {
        ciphertext += char(binaryVectorToDecimal(encrypt(hexToBinary(int(plaintext[i]), 16), key)));
    }
    return ciphertext;
}