#include <iostream>
#include "Matrix.h"

using namespace std;

Matrix :: Matrix() = default;

void Matrix::print() {
    int maxWidth = getMaxWidth();

    for (int row = 0; row < SIZE; row++) {
        cout << "|";
        for (int col = 0; col < SIZE; col++) {
            printf(" %*d ", maxWidth, matrix[row][col]);
        }
        cout << "|" << endl;
    }
}

void Matrix::read() {
    int num;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            cin >> num;
            matrix[row][col] = num;
        }
    }
}

int Matrix::getMaxWidth() {
    int maxWidth = 0;
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            int currentWidth = getDigitWidth(getMatrix(row, col));

            maxWidth = currentWidth > maxWidth ? currentWidth : maxWidth;
        }
    }
    return maxWidth;
}

int Matrix::getDigitWidth(int num) {
    // 0 입력 -> width 1 반환 해야함 ++ 안해도 알아서 자리 확보는 함
    if (num == 0) {return 1;}

    int width = 0;

    if (num < 0) {
        width = 1; // 음수(-) 자리 확보
        num *= -1;
    }

    for (; num != 0; num /= 10, width++) {}

    return width;
}

int Matrix::getMatrix(int row, int col) {
    return matrix[row][col];
}

void Matrix::setMatrix(int row, int col, int val) {
    matrix[row][col] = val;
}

Matrix Matrix::transpose() {
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(col,row,this->getMatrix(row,col));
        }
    }

    return resultMatrix;
}

Matrix Matrix::add(Matrix m) {
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(row,col, this->getMatrix(row,col) + m.getMatrix(row,col));
        }
    }

    return resultMatrix;
}

Matrix Matrix::multi(Matrix m) {
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(row,col,0);
            for (int cur = 0; cur < SIZE; cur++) {
                resultMatrix.setMatrix(row,col,
                    resultMatrix.getMatrix(row,col)+ this->getMatrix(row,cur)*m.getMatrix(cur,col));
            }
        }
    }
    return resultMatrix;
}
