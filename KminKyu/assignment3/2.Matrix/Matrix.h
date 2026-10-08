#pragma once
#include <iostream>
using namespace std;

class Matrix {
public:
    Matrix();

    Matrix transpose() const;
    Matrix add(const Matrix& otherMatrix) const;
    Matrix multi(const Matrix& otherMatrix) const;
    int getValue(int column, int row) const;
    void setValue(int column, int row, int value);
    const Matrix operator !() const;
    const Matrix operator +(const Matrix& matrix) const;
    const Matrix operator *(const Matrix& matrix) const;
    Matrix& operator =(const Matrix& matrix);
    static const int MAX_MATRIX_SIZE = 3;
    static int getLength(int);
    int getRowLength(int row) const;
private:
    int matrixArray[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = {0,};
};

ostream& operator <<(ostream&, const Matrix&);
istream& operator >>(istream&, Matrix&);