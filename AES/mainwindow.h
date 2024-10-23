#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    vector<int> encrypt(vector<int> plaintext, vector<int> key);
    vector<int> decrypt(vector<int> ciphertext, vector<int> key);

private slots:
    void on_EncryptButton_clicked();

private:
    Ui::MainWindow *ui;


    vector<int> hexToBinary(int hexValue);
    void printVector(const vector<int>& vec);
    vector<int> substituteNibble(vector<int> state);
    vector<int> inverseSubstituteNibble(vector<int>state);
    vector<int> shiftRows(vector<int>& state);
    vector<int> inverseShiftRows(vector<int>& state);
    void inverseMixColumns(vector<int>& state);
    void mixColumns(vector<int>& state);
    vector<int> RotNib(vector<int> state);
    vector<vector<int>> keyExpansion(vector<int> key);
};
#endif // MAINWINDOW_H
