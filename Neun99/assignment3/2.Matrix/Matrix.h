#pragma once
#include <iostream>

using namespace std;

class Matrix {
public:
    //getter setter
    int getValue(int, int) const;
    void setValue(int, int, int);

    //연산자 오버로딩
    const Matrix operator!() const;
    const Matrix operator+(const Matrix&) const;
    const Matrix operator*(const Matrix&) const;

    //배열 관련 ( << >> 오버로딩 때문에 public)
    static constexpr int SIZE = 3;
    int getMaxWidth() const;
private:
    int matrix[SIZE][SIZE] = {};
};

ostream& operator<<(ostream&, const Matrix&);
istream& operator>>(istream&, Matrix&);