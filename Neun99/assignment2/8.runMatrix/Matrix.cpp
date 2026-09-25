#include <iostream>
#include "Matrix.h"

using namespace std;

void Matrix::setValue(int rowIdx, int colIdx, int value) {
    matrix[rowIdx][colIdx] = value;
}

int Matrix::getValue(int rowIdx, int colIdx) {
    return matrix[rowIdx][colIdx];
}

void Matrix::read() {
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            int userInput;
            cin >> userInput;
            setValue(rowIdx, colIdx, userInput);
        }
    }
}

void Matrix::print() {
    int width = getMaxWidth();
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        cout << "| ";
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            printf("%*d ",width, matrix[rowIdx][colIdx]);
        }
        cout << "|" << endl;
    }
}

Matrix Matrix::transpose() {
    Matrix resultMatrix;

    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            resultMatrix.setValue(rowIdx, colIdx, getValue(colIdx, rowIdx));
        }
    }

    return resultMatrix;
}

Matrix Matrix::add(Matrix& matrix2) {
    Matrix resultMatrix;

    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            int value = getValue(rowIdx, colIdx) + matrix2.getValue(rowIdx, colIdx);
            resultMatrix.setValue(rowIdx, colIdx, value);
        }
    }

    return resultMatrix;
}

Matrix Matrix::multi(Matrix& matrix2) {
    Matrix resultMatrix;

    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            for (int idx = 0; idx < SIZE; idx++) {
                int value = resultMatrix.getValue(rowIdx, colIdx);
                value += getValue(rowIdx, idx) * matrix2.getValue(idx, colIdx);
                resultMatrix.setValue(rowIdx, colIdx, value);
            }
        }
    }

    return resultMatrix;
}

int Matrix::getMaxWidth() {
    int maxWidth = 0;
    for (int rowIdx = 0; rowIdx < SIZE; rowIdx++) {
        for (int colIdx = 0; colIdx < SIZE; colIdx++) {
            int value = getValue(rowIdx, colIdx);
            int width = 1;

            if (value < 0) //value가 음수면 마이너스 출력할 한 자리 추가
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
