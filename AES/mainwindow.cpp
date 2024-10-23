#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString styleSheet = R"(
        QMainWindow {
            background-color: #f0f0f0;  /* 主窗口背景颜色 */
        }

        QLabel {
            font-size: 24pt;  /* 标签字体大小 */
            font-weight: bold;  /* 标签字体加粗 */
            color: #333333;  /* 标签字体颜色 */
            padding: 10px;  /* 标签内边距 */
        }

        QTextEdit {
            background-color: #ffffff;  /* 文本编辑框背景颜色 */
            border: 2px solid #cccccc;  /* 边框颜色 */
            border-radius: 5px;  /* 圆角边框 */
            font-family: 'SimSun';  /* 字体 */
            font-size: 14pt;  /* 字体大小 */
            padding: 5px;  /* 内边距 */
        }

        QPushButton {
            background-color: #4CAF50;  /* 按钮背景颜色 */
            color: white;  /* 按钮字体颜色 */
            border: none;  /* 去掉边框 */
            border-radius: 5px;  /* 圆角 */
            padding: 10px;  /* 按钮内边距 */
            font-size: 16pt;  /* 按钮字体大小 */
        }

        QPushButton:hover {
            background-color: #45a049;  /* 鼠标悬停时颜色 */
        }

        QTextEdit[readOnly="true"] {
            background-color: #eeeeee;  /* 只读文本框背景颜色 */
        }
    )";

    this->setStyleSheet(styleSheet);  // 在 MainWindow 构造函数中应用样式表
}

MainWindow::~MainWindow()
{
    delete ui;
}


//十六进制数转换为2进制数
vector<int> MainWindow::hexToBinary(int hexValue) {
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
void MainWindow::printVector(const vector<int>& vec) {
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
vector<int> MainWindow::substituteNibble(vector<int> state) {
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
vector<int> MainWindow::inverseSubstituteNibble(vector<int>state) {
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
vector<int> MainWindow::shiftRows(vector<int>& state) {
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
vector<int> MainWindow::inverseShiftRows(vector<int>& state) {
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
void MainWindow::inverseMixColumns(vector<int>& state) {
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
void MainWindow::mixColumns(vector<int>& state) {
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
vector<int>MainWindow::RotNib(vector<int> state) {
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
vector<vector<int>> MainWindow::keyExpansion(vector<int> key) {
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

vector<int> binaryStringToVector(const std::string& str) {
    std::vector<int> vec;

    for (char ch : str) {
        if (ch == '0' || ch == '1') {
            vec.push_back(ch - '0'); // 将字符 '0' 或 '1' 转换为整数 0 或 1
        } else {
            std::cerr << "Invalid character: " << ch << std::endl;
        }
    }

    return vec;
}

string vectorToBinaryString(const std::vector<int>& vec) {
    std::string str;

    for (int num : vec) {
        if (num == 0 || num == 1) {
            str.push_back(num + '0'); // 将 0 或 1 转换为字符 '0' 或 '1'
        } else {
            std::cerr << "Invalid number in vector: " << num << std::endl;
        }
    }

    return str;
}


// 加密函数
vector<int> MainWindow::encrypt(vector<int> plaintext, vector<int> key) {
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
vector<int> MainWindow::decrypt(vector<int> ciphertext, vector<int> key) {
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

void MainWindow::on_EncryptButton_clicked() {
    //处理明文
    QString text_1 = ui-> PlainText-> toPlainText();
    string plain_text = text_1.toStdString();
    vector<int> plain_vector = binaryStringToVector(plain_text);

    //处理密钥
    QString text_2 = ui-> Key-> toPlainText();
    string key_text = text_2.toStdString();
    vector<int> key = binaryStringToVector(key_text);

    vector<int> result = encrypt(plain_vector, key);
    string cipher_text = vectorToBinaryString(result);
    QString text_3 = QString::fromStdString(cipher_text);
    ui->CipherText->clear();
    ui->CipherText->setPlainText(text_3);

}


