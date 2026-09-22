#pragma once

class Matrix
{
public:
    void setValue(int rowIdx, int colIdx, int value);
    int getValue(int rowIdx, int colIdx);
    void read();
    void print();
    Matrix transpose();
    Matrix add(Matrix matrix);
    Matrix multi(Matrix matrix);

private:
    static const int MAX_SIZE = 3;
    int matrix[MAX_SIZE][MAX_SIZE];
};