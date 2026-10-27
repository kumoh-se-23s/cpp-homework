#include <iostream>
#include "MyMatrix.h"

using namespace std;

//기본 생성자
MyMatrix::MyMatrix() {
    matrix = nullptr;
    rowSize = 0;
    colSize = 0;
}

//크기 받는 생성자
MyMatrix::MyMatrix(int row, int col) {
    rowSize = row;
    colSize = col;

    matrix = new int*[rowSize];
    for (int idx = 0; idx < rowSize; idx++) {
        matrix[idx] = new int[colSize];
    }
}

//복사생성자
MyMatrix::MyMatrix(const MyMatrix &mat) {
    rowSize = mat.rowSize;
    colSize = mat.colSize;

    matrix = new int*[rowSize];
    copyMat(matrix, mat.matrix, rowSize, colSize);
}

//소멸
MyMatrix::~MyMatrix() {
    for (int idx = 0; idx < rowSize; idx++) {
        delete[] matrix[idx];
    }
    delete[] matrix;
}

//private--------------------------------------------
//행렬 복사
void MyMatrix::copyMat(int **copy, const int **src, int rowSize, int colSize) {
    for (int rowIdx = 0; rowIdx < rowSize; rowIdx++) {
        for (int colIdx = 0; colIdx < colSize; colIdx++) {
            copy[rowIdx][colIdx] = src[rowIdx][colIdx];
        }
    }
}

//배열 복사
void MyMatrix::copyArr(int *copy, const int *src, int srcSize) {
    for (int idx = 0; idx < srcSize; idx++) {
        copy[idx] = src[idx];
    }
}
