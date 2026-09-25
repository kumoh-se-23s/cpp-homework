#pragma once
class Matrix {
public:
    void setValue(int, int, int);
    int getValue(int, int);
    void read();
    void print();
    Matrix transpose();
    Matrix add(Matrix&);
    Matrix multi(Matrix&);
private:
    static const int SIZE = 3;
    int matrix[SIZE][SIZE] = {};
    int maxWidth();
};