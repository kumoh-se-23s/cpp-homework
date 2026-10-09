#pragma once
#include <iostream>

using namespace std;

class Matrix {
public:
    //getter setter
    int getValue(int rowIdx, int colIdx) const;
    void setValue(int rowIdx, int colIdx, int value);

    //연산자 오버로딩
    const Matrix operator!() const;
    const Matrix operator+(const Matrix& mat2) const;
    const Matrix operator*(const Matrix& mat2) const;

    //배열 관련 ( << >> 오버로딩 때문에 public)
    static constexpr int SIZE = 3;
    int getMaxWidth() const;
private:
    int matrix[SIZE][SIZE] = {};
};

ostream& operator<<(ostream& out, const Matrix& mat);
istream& operator>>(istream& in, Matrix& mat);