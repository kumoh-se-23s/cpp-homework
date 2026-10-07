#include <iostream>
#include "Matrix.h"
#include <iomanip>

using namespace std;

//getter setter----------------
int Matrix::getValue(int rowIdx, int colIdx) const{
    return matrix[rowIdx][colIdx];
}

void Matrix::setValue(int rowIdx, int colIdx, int value) {
    if (rowIdx < SIZE && colIdx < SIZE)
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
        out << "| ";
        for (int colIdx = 0; colIdx < Matrix::SIZE; colIdx++) {
            out << setw(width) << mat.getValue(rowIdx, colIdx);
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
            int value = getValue(rowIdx, colIdx);
            int width = 1;

            if (value < 0) //value가 음수면 마이너스 출력할 한자리 추가
                width++;

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