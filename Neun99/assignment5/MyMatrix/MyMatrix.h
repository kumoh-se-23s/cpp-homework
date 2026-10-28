#pragma once
#include <iostream>

using namespace std;

class MyMatrix {
public:
    MyMatrix();
    MyMatrix(int row, int col);
    MyMatrix(const MyMatrix &mat);
    ~MyMatrix();

    int getRowSize() const;
    int getColSize() const;
    int getValue(int rowPos, int colPos) const;
    int getMaxWidth() const;

    void init(const int *arr);

    const MyMatrix operator+(const MyMatrix &mat2) const;
    const MyMatrix operator*(const MyMatrix &mat2) const;
    MyMatrix& operator=(const MyMatrix &mat2);
private:
    int rowSize;
    int colSize;
    int **matrix;

    static MyMatrix getFailMatrix();
    static void copyMat(int **copy, const int * const*src, int rowSize, int colSize);
    static void copyArr(int *copy, const int *src, int srcSize);
};

ostream& operator<<(ostream &out, const MyMatrix &mat);