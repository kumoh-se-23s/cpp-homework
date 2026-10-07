//
// Created by apsode on 26. 10. 7..
//
#include <iostream>
using namespace std;

class Matrix {
public :
    Matrix() ;
    Matrix(int r, int c, int initValue = 0);
    int getItem(int row, int col) const;
    void setItem(int row, int col, int val);
    ~Matrix();
private :
    int rowSize ;
    int colSize ;
    int* arr;
};

Matrix::Matrix() : rowSize(0), colSize(0), arr(0) {}

Matrix::Matrix(int r, int c, int initValue = 0)
{
    arr = new int[r * c];
    for (int i = 0; i < r * c; i++)
        arr[i] = initValue;
}

int Matrix::getItem(int row, int col) const {
    if (0 <= row && row < rowSize && 0 <= col && col < colSize)
        return arr[row * colSize + col];
    else
        throw out_of_range("배열 인덱스의 범위 초과");
    // 또는 return 0 ;
}

void Matrix::setItem(int row, int col, int val)  {
    if (0 <= row && row < rowSize && 0 <= col && col < colSize)
        arr[row * colSize + col] = val;
    else
        throw out_of_range("배열 인덱스의 범위 초과");
    // 또는 return ;
}

Matrix::~Matrix() { delete[] arr; }