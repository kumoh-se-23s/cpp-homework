#include<iostream>
#include "Matrix.h"

using namespace std;

int getLength(int number) {
    int cnt = 1;
    while (number / 10 != 0) {
        number /= 10;
        ++cnt;
    }
    cout << cnt;
    return cnt;
}

Matrix::Matrix() {
}

void Matrix::read() {
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            cin >> matrixArray[column][row];
        }
    }
}

int Matrix::getValue(int column, int row) const {
    return matrixArray[column][row];
}

void Matrix::setValue(int column, int row, int value) {
    matrixArray[column][row] = value;
}

int Matrix::getMaxLengthValue() const {
    int maxLengthNumber = 0;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            if (abs(maxLengthNumber) < abs(matrixArray[column][row])) {
                maxLengthNumber = matrixArray[column][row];
            }
        }
    }
    return maxLengthNumber;
}

Matrix Matrix::transpose() const {
    Matrix resultMatrix;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            resultMatrix.setValue(row, column, matrixArray[column][row]);
        }
    }
    return resultMatrix;
}

Matrix Matrix::add(const Matrix& otherMatrix) const {
    Matrix resultMatrix;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            resultMatrix.setValue(column, row, this->getValue(column, row) + otherMatrix.getValue(column, row));
        }
    }
    return resultMatrix;
}

Matrix Matrix::multi(const Matrix& otherMatrix) const {
    Matrix resultMatrix;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            int result = 0;
            for (int indexCount = 0; indexCount < MAX_MATRIX_SIZE; ++indexCount) {
                result += this->getValue(column, indexCount) * otherMatrix.getValue(indexCount, row);
            }
            resultMatrix.setValue(column, row, result);
        }
    }
    return resultMatrix;
}

void Matrix::print() const {
    int maxLength = getLength(this->getMaxLengthValue());
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            if (row == 0) {
                cout << "| ";
            }
            printf("%*d", maxLength, this->getValue(column, row));

            cout << " ";
            if (row == MAX_MATRIX_SIZE - 1) {
                cout << "|";
            }
        }
        cout << endl;
    }
}
