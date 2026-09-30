#include <iostream>

using namespace std;

#pragma once
class Matrix {
public:
    void setValue(int, int, int);
    int getValue(int, int) const;
    const Matrix operator!() const;
    const Matrix operator+(const Matrix&) const;
    const Matrix operator*(const Matrix&) const;
    static const int SIZE = 3;
    const int getMaxWidth() const;
private:
    int matrix[SIZE][SIZE] = {};
};

ostream& operator<<(ostream&, const Matrix&);
istream& operator>>(istream&, Matrix&);