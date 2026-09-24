#pragma once;

class Matrix {
public:
    Matrix();
    void read();
    void print();
    Matrix transpose();
    Matrix add(Matrix m);
    Matrix multi(Matrix m);

private:
    int getMatrix(int row, int col);
    void setMatrix(int row, int col, int val);
    int getMaxWidth();
    int getDigitWidth(int num);
    const static int SIZE = 3;
    int matrix[SIZE][SIZE] = {};
};
