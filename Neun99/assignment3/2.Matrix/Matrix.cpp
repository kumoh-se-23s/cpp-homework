#include <iostream>
#include "Matrix.h"
#include <iomanip>

using namespace std;

void Matrix::setValue(int rowIdx, int colIdx, int value) {
    if (rowIdx < SIZE && colIdx < SIZE)
        matrix[rowIdx][colIdx] = value;
}

int Matrix::getValue(int rowIdx, int colIdx) const{
    return matrix[rowIdx][colIdx];
}
//연산자오버로딩----------------------------------------------
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

//단항!
const Matrix Matrix::operator!() const{
    Matrix resultMatrix;

    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            resultMatrix.setValue(rowIdx, colIdx, getValue(colIdx, rowIdx));
        }
    }

    return resultMatrix;
}

//+
const Matrix Matrix::operator+(const Matrix& matrix2) const{
    Matrix resultMatrix;

    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            int value = getValue(rowIdx, colIdx) + matrix2.getValue(rowIdx, colIdx);
            resultMatrix.setValue(rowIdx, colIdx, value);
        }
    }

    return resultMatrix;
}

//*
const Matrix Matrix::operator*(const Matrix& matrix2) const{
    Matrix resultMatrix;
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            int value = 0;
            for (int idx = 0; idx < SIZE; idx++) {
                value += getValue(rowIdx, idx) * matrix2.getValue(idx, colIdx);
            }
            resultMatrix.setValue(rowIdx, colIdx, value);
        }
    }

    return resultMatrix;
}


const int Matrix::getMaxWidth() const{
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