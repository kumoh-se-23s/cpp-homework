#pragma once
#include <iostream>

using namespace std;

class MyMatrix {
public:
    MyMatrix();
    MyMatrix(int row, int col);
    MyMatrix(const MyMatrix &mat);
    ~MyMatrix();
    void init(int *arr);

    const MyMatrix operator+(const MyMatrix &mat2) const;
    const MyMatrix operator*(const MyMatrix &mat2) const;
private:
    int rowSize;
    int colSize;
    int **matrix;

    static void copyMat(int **copy, const int **src, int rowSize, int colSize);
    static void copyArr(int *copy, const int *src, int srcSize);
};

ostream& operator<<(ostream &out, const MyMatrix &mat);