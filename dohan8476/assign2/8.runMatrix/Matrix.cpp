#include <iostream>
#include "Matrix.h"

using namespace std;

Matrix::Matrix() {
    int Matrix[SIZE][SIZE] = {};
}

void Matrix::print() {
    for (int i = 0; i < SIZE; i++) {
        cout << "|";
        for (int j = 0; j < SIZE; j++) {
            printf("%2d", matrix[i][j]);
            // printf("%0*d", maxWidth, matrix[i][j]);
        }
        cout << "|" << endl;
    }
}

void Matrix::read() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> matrix[i][j];
        }
    }
}

int Matrix::getMaxWidth() {
    int maxWidth;
}

void Matrix::setMaxWidth(int maxWidth) {
    maxWidth = maxWidth;
}

int Matrix::getMatrix(int row, int col) {
    return matrix[row][col];
}

void Matrix::setMatrix(int row, int col, int val) {
    matrix[row][col] = val;
}

Matrix Matrix::transpose() {
    Matrix resultMatrix;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            resultMatrix.setMatrix(j, i, this->getMatrix(i, j));
        }
    }

    return resultMatrix;
}

Matrix Matrix::add(Matrix m) {
    Matrix resultMatrix;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            resultMatrix.setMatrix(i, j, this->getMatrix(i, j) + m.getMatrix(i, j));
        }
    }

    return resultMatrix;
}

Matrix Matrix::multi(Matrix m) {
    Matrix resultMatrix;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            resultMatrix.setMatrix(i, j, 0);
            for (int k = 0; k < SIZE; k++) {
                resultMatrix.setMatrix(i, j,
                                       resultMatrix.getMatrix(i, j) + this->getMatrix(i, k) * m.getMatrix(k, j));
            }
        }
    }

    return resultMatrix;
}
