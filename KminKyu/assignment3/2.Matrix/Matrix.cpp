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
    if (0 <= column && column < MAX_MATRIX_SIZE &&
        0 <= row && row < MAX_MATRIX_SIZE) {
        return matrixArray[column][row];
    } else {
        return 0;
    }

}

void Matrix::setValue(int column, int row, int value) {
    if (0 <= column && column < MAX_MATRIX_SIZE &&
        0 <= row && row < MAX_MATRIX_SIZE) {

        matrixArray[column][row] = value;

        value = value < 0 ? abs(value * -10) : value;
        maxValues[row] = maxValues[row] > value ? maxValues[row] : value;
    }
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

Matrix& Matrix::operator =(const Matrix& matrix) {

    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            matrixArray[column][row] = matrix.getValue(column, row);
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