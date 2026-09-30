#include <iostream>

using namespace std;

#pragma once
class Matrix {
public:
    void setValue(int, int, int);
    int getValue(int, int);
    Matrix operator!();
    Matrix operator+(Matrix&);
    Matrix operator*(Matrix&);
    static const int SIZE = 3;
    int getMaxWidth();
private:
    int matrix[SIZE][SIZE] = {};
};

ostream& operator<<(ostream&, Matrix&);
istream& operator>>(istream&, Matrix&);