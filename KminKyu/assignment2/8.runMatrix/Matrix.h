#pragma once

class Matrix {
public:
    Matrix();

    void read();
    void print() const;
    Matrix transpose() const;
    Matrix add(const Matrix& otherMatrix) const;
    Matrix multi(const Matrix& otherMatrix) const;
    int getValue(int column, int row) const;
    void setValue(int column, int row, int value);

private:
    static const int MAX_MATRIX_SIZE = 3;
    static int getLength(int);
    int getMaxLengthValue() const;
    int matrixArray[3][3] = {0,};
};