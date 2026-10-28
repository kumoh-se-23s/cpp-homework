#include <iostream>
#include <iomanip>
#include "MyMatrix.h"
#include <limits>

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
    for (int idx = 0; idx < rowSize; idx++) {
        matrix[idx] = new int[colSize];
    }

    copyMat(matrix, mat.matrix, rowSize, colSize);
}

//소멸
MyMatrix::~MyMatrix() {
    for (int idx = 0; idx < rowSize; idx++) {
        delete[] matrix[idx];
    }
    delete[] matrix;
}
//getter---------------------------------------------
int MyMatrix::getRowSize() const {
    return rowSize;
}

int MyMatrix::getColSize() const {
    return colSize;
}

int MyMatrix::getValue(int rowPos, int colPos) const {
    if (rowPos < 0 || rowPos >= rowSize || colPos < 0 || colPos >= colSize)
        return numeric_limits<int>::min();
    return matrix[rowPos][colPos];
}

//배열 내 최대 길이 반환
int MyMatrix::getMaxWidth() const{
    int maxWidth = 0;
    for (int rowIdx = 0; rowIdx < rowSize; rowIdx++) {
        for (int colIdx = 0; colIdx < colSize; colIdx++) {
            int value = matrix[rowIdx][colIdx];
            int width = 3; //자릿수 + 부호공간 + 1

            for (; value >= 10 || value <= -10; value /= 10) { //자릿수 구하기
                width++;
            }

            if (width > maxWidth) {
                maxWidth = width;
            }
        }
    }
    return maxWidth;
}

//기능------------------------------------------------
//개수 맞춰 들어왔다고 가정
void MyMatrix::init(const int *arr) {
    for (int rowIdx = 0; rowIdx < rowSize; rowIdx++) {
        for (int colIdx = 0; colIdx < colSize; colIdx++) {
            matrix[rowIdx][colIdx] = *(arr + (rowIdx*colSize) + colIdx);
        }
    }
}

//연산자 오버로딩---------------------------------------
const MyMatrix MyMatrix::operator+(const MyMatrix& mat2) const{
    //행렬 크기 안 맞으면 탈띾!
    if (rowSize != mat2.rowSize || colSize != mat2.colSize) {
        return getFailMatrix();
    }

    MyMatrix resultMatrix = MyMatrix(rowSize, colSize);
    //합
    for (int rowIdx = 0; rowIdx < rowSize; rowIdx++) {
        for (int colIdx = 0; colIdx < colSize; colIdx++) {
            resultMatrix.matrix[rowIdx][colIdx] = matrix[rowIdx][colIdx] + mat2.matrix[rowIdx][colIdx];
        }
    }
    return resultMatrix;
}

const MyMatrix MyMatrix::operator*(const MyMatrix& mat2) const{
    //행렬 곱셈 조건 안 맞으면 탈락!
    if (colSize != mat2.rowSize) {
        return getFailMatrix();
    }

    MyMatrix resultMatrix = MyMatrix(rowSize, mat2.colSize);
    //곱
    for (int rowIdx = 0; rowIdx < rowSize; rowIdx++) {
        for (int colIdx = 0; colIdx < mat2.colSize; colIdx++) {
            int value = 0;
            for (int idx = 0; idx < colSize; idx++) {
                value += matrix[rowIdx][idx] * mat2.matrix[idx][colIdx];
            }
            resultMatrix.matrix[rowIdx][colIdx] = value;
        }
    }
    return resultMatrix;
}

//대입
MyMatrix &MyMatrix::operator=(const MyMatrix &mat2) {
    if (this == &mat2)
        return *this;

    for (int idx = 0; idx < rowSize; idx++) {
        delete[] matrix[idx];
    }
    delete[] matrix;

    rowSize = mat2.rowSize;
    colSize = mat2.colSize;

    matrix = new int*[rowSize];
    for (int idx = 0; idx < rowSize; idx++) {
        matrix[idx] = new int[colSize];
    }

    copyMat(matrix, mat2.matrix, rowSize, colSize);
    return *this;
}

ostream &operator<<(ostream &out, const MyMatrix &mat) {
    int width = mat.getMaxWidth();
    for (int rowIdx = 0; rowIdx < mat.getRowSize(); rowIdx++) {
        out << "|";
        for (int colIdx = 0; colIdx < mat.getColSize(); colIdx++) {
            out << setw(width) << mat.getValue(rowIdx, colIdx) << " ";
        }
        out << "|" << endl;
    }
    return out;
}

//private--------------------------------------------
//1x1짜리 -999 행렬 리턴
MyMatrix MyMatrix::getFailMatrix() {
    MyMatrix resultMatrix = MyMatrix(1,1);
    resultMatrix.matrix[0][0] = -999;
    return resultMatrix;
}

//행렬 복사
void MyMatrix::copyMat(int **copy, const int* const* src, int rowSize, int colSize) {
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