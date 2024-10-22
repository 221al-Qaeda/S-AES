#include <iostream>
#include <vector>
using namespace std;

//十六进制数转换为2进制数
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
//输出vector
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




// 替换函数
vector<int> substituteNibble(vector<int> state) {
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
        vector<int> nibble = hexToBinary(sbox[row1][col1]);
        result.insert(result.end(), nibble.begin(), nibble.end());
    }

    return result;
}

// 逆半字节替代函数
vector<int> inverseSubstituteNibble(vector<int>state) {
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
        vector<int> nibble = hexToBinary(invSbox[row1][col1]);
        result.insert(result.end(), nibble.begin(), nibble.end());
    }

    return result;
}

// 行移位函数
vector<int> shiftRows(vector<int>& state) {
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

// 逆行移位函数
vector<int> inverseShiftRows(vector<int>& state) {
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

// 逆列混淆函数
void inverseMixColumns(vector<int>& state) {
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

// 列混淆函数
void mixColumns(vector<int>& state) {
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

// 轮常数
const vector<int>rocn1 = { 1,0,0,0,0,0,0,0 };
const vector<int>rocn2 = { 0,0,1,1,0,0,0,0 };
//交换前后
vector<int> RotNib(vector<int> state) {
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
// 密钥拓展函数
vector<vector<int>> keyExpansion(vector<int> key) {
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

// 加密函数
vector<int> encrypt(vector<int> plaintext, vector<int> key) {
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

// 解密函数
vector<int> decrypt(vector<int> ciphertext, vector<int> key) {
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

int main() {
    vector<int> plaintext = { 1,0,1,0,0,1,1,1,0,1,0,0,1,0,0,1 };
    vector<int> key = { 0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1 };
    cout << "明文为：";
    printVector(plaintext);
    vector<int>state = encrypt(plaintext, key);
    cout << "密文为：";
    printVector(state);
    vector<int>mingwen = decrypt(state, key);
    cout << "经密文解密后得到的明文：";
    printVector(mingwen);
    return 0;
}