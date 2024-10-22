#include <iostream>
#include <vector>
using namespace std;

//ʮ��������ת��Ϊ2������
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
//���vector
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




// �滻����
vector<int> substituteNibble(vector<int> state) {
    // ��� state �ĳ����Ƿ���4�ı���
    if (state.size() % 4 != 0) {
        cerr << "State size must be a multiple of 4." << endl;
        return {};
    }

    vector<int> result;
    result.reserve(state.size());

    // ��ÿ�ĸ�λ��Ϊһ������滻
    for (size_t i = 0; i < state.size(); i += 4) {
        int row1 = state[i] * 2 + state[i + 1];
        int col1 = state[i + 2] * 2 + state[i + 3];
        vector<int> nibble = hexToBinary(sbox[row1][col1]);
        result.insert(result.end(), nibble.begin(), nibble.end());
    }

    return result;
}

// ����ֽ��������
vector<int> inverseSubstituteNibble(vector<int>state) {
    // ��� state �ĳ����Ƿ���4�ı���
    if (state.size() % 4 != 0) {
        cerr << "State size must be a multiple of 4." << endl;
        return {};
    }

    vector<int> result;
    result.reserve(state.size());

    // ��ÿ�ĸ�λ��Ϊһ������滻
    for (size_t i = 0; i < state.size(); i += 4) {
        int row1 = state[i] * 2 + state[i + 1];
        int col1 = state[i + 2] * 2 + state[i + 3];
        vector<int> nibble = hexToBinary(invSbox[row1][col1]);
        result.insert(result.end(), nibble.begin(), nibble.end());
    }

    return result;
}

// ����λ����
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

// ������λ����
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

// ���л�������
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

// �л�������
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

// �ֳ���
const vector<int>rocn1 = { 1,0,0,0,0,0,0,0 };
const vector<int>rocn2 = { 0,0,1,1,0,0,0,0 };
//����ǰ��
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
// ��Կ��չ����
vector<vector<int>> keyExpansion(vector<int> key) {
    vector<vector<int>> w(6, vector<int>(8, 0));
    //������ key ��ǰ8λ�洢�� w[0] ��
    for (int i = 0; i < 8; i++) {
        w[0][i] = key[i];
    }

    // ������ key �ĺ�8λ�洢�� w[1] ��
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

// ���ܺ���
vector<int> encrypt(vector<int> plaintext, vector<int> key) {
    vector<int> state = plaintext;
    vector<vector<int>>Key;

    Key = keyExpansion(key);
    // ��ʼ����Կ��
    //cout << "��Կ0��";
    //printVector(Key[0]);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[0][i];
    }
    //cout << "��ʼ����Կ��:";
    //printVector(state);
    // ��һ��
    state = substituteNibble(state);
    //cout << "���ֽ��滻��";
    //printVector(state);
    shiftRows(state);
    //cout << "�б仯��";
    //printVector(state);
    mixColumns(state);
    //cout << "�л�����";
    //printVector(state);
    //cout << "��Կ1��";
    //printVector(Key[1]);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[1][i];
    }
    //cout << "��һ������Կ��:";
    //printVector(state);
    // �ڶ���

    state = substituteNibble(state);
    //cout << "���ֽ��滻��";
    //printVector(state);
    shiftRows(state);
    //cout << "�б仯��";
    //printVector(state);
    //cout << "��Կ2��";
    //printVector(Key[2]);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[2][i];
    }
    //cout << "�ڶ�������Կ��:";
    //printVector(state);
    return state;
}

// ���ܺ���
vector<int> decrypt(vector<int> ciphertext, vector<int> key) {
    vector<int> state = ciphertext;
    vector<vector<int>>Key;
    //cout << "�����ǣ�"<<endl;
    //printVector(ciphertext);
    Key = keyExpansion(key);
    // ��ʼ����Կ��
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[2][i];
    }
    //cout << "��ʼ����Կ��:";
    //printVector(state);
    // ��һ����
    inverseShiftRows(state);
    //cout << "�б仯��";
    //printVector(state);
    state = inverseSubstituteNibble(state);
    for (int i = 0; i < 16; i++) {
        state[i] ^= Key[1][i];
    }
    inverseMixColumns(state);

    // �ڶ�����
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
    cout << "����Ϊ��";
    printVector(plaintext);
    vector<int>state = encrypt(plaintext, key);
    cout << "����Ϊ��";
    printVector(state);
    vector<int>mingwen = decrypt(state, key);
    cout << "�����Ľ��ܺ�õ������ģ�";
    printVector(mingwen);
    return 0;
}