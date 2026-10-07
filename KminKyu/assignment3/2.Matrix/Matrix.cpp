#include<iostream>
#include <iomanip>
#include "Matrix.h"

using namespace std;

Matrix::Matrix() = default;

int Matrix::getLength(int number) {
    int cnt = (number < 0) ? 1 : 0;
    for (; number != 0; number /= 10) {
        ++cnt;
    }
    return cnt;
}

int Matrix::getRowLength(int row) const {
    if (0 <= row && row < MAX_MATRIX_SIZE) {
        return getLength(maxValues[row]);
    } else {
        return 0;
    }
}

int Matrix::getValue(int column, int row) const {
    return matrixArray[column][row];
}

void Matrix::setValue(int column, int row, int value) {
    if (column < MAX_MATRIX_SIZE && row < MAX_MATRIX_SIZE) {
        matrixArray[column][row] = value;
    }
}


Matrix Matrix::transpose() const {
    Matrix resultMatrix;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            resultMatrix.matrixArray[row][column] = matrixArray[column][row];
        }
    }
    return resultMatrix;
}

Matrix Matrix::add(const Matrix& otherMatrix) const {
    Matrix resultMatrix;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            resultMatrix.matrixArray[column][row] =
                this->matrixArray[column][row] + otherMatrix.matrixArray[column][row];
        }
    }
    return resultMatrix;
}

Matrix Matrix::multi(const Matrix& otherMatrix) const {
    Matrix resultMatrix;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            for (int indexCount = 0; indexCount < MAX_MATRIX_SIZE; ++indexCount) {
                resultMatrix.matrixArray[column][row] +=
                    this->matrixArray[column][row] + otherMatrix.matrixArray[column][row];
            }
        }
    }
    return resultMatrix;
}

Matrix& Matrix::operator =(const Matrix& matrix) {

    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            matrixArray[column][row] = matrix.matrixArray[column][row];
        }
    }
    return *this;
}
const Matrix Matrix::operator +(const Matrix& matrix) const {
    return this->add(matrix);
}
const Matrix Matrix::operator !() const {
    return this->transpose();
}

const Matrix Matrix::operator *(const Matrix& matrix) const {
    return this->multi(matrix);
}

ostream& operator <<(ostream& out, const Matrix& matrix) {
    for (int column = 0; column < Matrix::MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < Matrix::MAX_MATRIX_SIZE; ++row) {

            if (row == 0) {
                out << "| ";
            }
            out << setw(matrix.getRowLength(row)) << matrix.getValue(column, row);
            out << " ";
            if (row == Matrix::MAX_MATRIX_SIZE - 1) {
                out << "|";
            }
        }
        out << "\n";
    }
    return out;
}

istream& operator >>(istream& in, Matrix& matrix){
    int inputValue;
    for (int column = 0; column < Matrix::MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < Matrix::MAX_MATRIX_SIZE; ++row) {
            in >> inputValue;
            matrix.setValue(column, row, inputValue);
        }
    }
    return in;
}