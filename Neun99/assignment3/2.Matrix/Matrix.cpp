#include <iostream>
#include "Matrix.h"
#include <iomanip>

using namespace std;

//getter setter----------------
int Matrix::getValue(int rowIdx, int colIdx) const{
    if (rowIdx < SIZE && colIdx < SIZE && rowIdx >= 0 && colIdx >= 0)
        return matrix[rowIdx][colIdx];
    return -2123456789; //범위가 int전체라 뭘 return해도 정상범위랑 겹치지만, 최대한 안 나올거 같은 숫자로..
}

void Matrix::setValue(int rowIdx, int colIdx, int value) {
    if (rowIdx < SIZE && colIdx < SIZE && rowIdx >= 0 && colIdx >= 0)
        matrix[rowIdx][colIdx] = value;
}
//연산자오버로딩----------------------------------------------
//단항!
const Matrix Matrix::operator!() const{
    Matrix resultMatrix;
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            resultMatrix.matrix[rowIdx][colIdx] = matrix[colIdx][rowIdx];
        }
    }
    return resultMatrix;
}

//+
const Matrix Matrix::operator+(const Matrix& mat2) const{
    Matrix resultMatrix;
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            resultMatrix.matrix[rowIdx][colIdx] = matrix[rowIdx][colIdx] + mat2.matrix[rowIdx][colIdx];
        }
    }
    return resultMatrix;
}

//*
const Matrix Matrix::operator*(const Matrix& mat2) const{
    Matrix resultMatrix;
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            int value = 0;
            for (int idx = 0; idx < SIZE; idx++) {
                value += matrix[rowIdx][idx] * mat2.matrix[idx][colIdx];
            }
            resultMatrix.matrix[rowIdx][colIdx] = value;
        }
    }
    return resultMatrix;
}

//>>
istream& operator>>(istream& in, Matrix& mat) {
    for (int rowIdx = 0; rowIdx < Matrix::SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < Matrix::SIZE; colIdx++) {
            int userInput;
            in >> userInput;
            mat.setValue(rowIdx, colIdx, userInput);
        }
    }
    return in;
}

//<<
ostream& operator<<(ostream& out, const Matrix& mat) {
    int width = mat.getMaxWidth();
    for (int rowIdx = 0; rowIdx < Matrix::SIZE; rowIdx++) {
        out << "|";
        for (int colIdx = 0; colIdx < Matrix::SIZE; colIdx++) {
            out << setw(width) << mat.getValue(rowIdx, colIdx) << " ";
        }
        out << "|" << endl;
    }
    return out;
}

//배열 내 최대 길이 반환
int Matrix::getMaxWidth() const{
    int maxWidth = 0;
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
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