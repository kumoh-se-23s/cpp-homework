#include<iostream>
#include "Matrix.h"

#include <iomanip>

using namespace std;

Matrix::Matrix() = default;

int Matrix::getLength(int number) {
    int cnt = 0;
    for (; number != 0; number /= 10) {
        ++cnt;
    }
    return cnt;
}

int Matrix::getValue(int column, int row) const {
    return matrixArray[column][row];
}

void Matrix::setValue(int column, int row, int value) {
    matrixArray[column][row] = value;
}

int Matrix::getMaxLengthValue(int row) const {
    int maxLengthNumber = 0;
    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {

        int tempNumber = matrixArray[column][row] < 0 ? matrixArray[column][row] * -10 : matrixArray[column][row];

        if (maxLengthNumber < tempNumber) {
            maxLengthNumber = tempNumber;
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

ostream& operator<<(ostream& out, const Matrix& matrix) {
    int maxLengthArray[Matrix::MAX_MATRIX_SIZE];
    for (int row = 0; row < Matrix::MAX_MATRIX_SIZE; ++row) {
        maxLengthArray[row] = Matrix::getLength(matrix.getMaxLengthValue(row));
        cout<<maxLengthArray[row]<<"\n";
    }
    for (int column = 0; column < Matrix::MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < Matrix::MAX_MATRIX_SIZE; ++row) {

            if (row == 0) {
                out << "| ";
            }
            out << setw(maxLengthArray[row]) << matrix.getValue(column, row);
            out << " ";
            if (row == Matrix::MAX_MATRIX_SIZE - 1) {
                out << "|";
            }
        }
        out << "\n";
    }
    return out;
}

istream& operator>>(istream& in, Matrix& matrix){
    int inputValue;
    for (int column = 0; column < Matrix::MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < Matrix::MAX_MATRIX_SIZE; ++row) {
            in >> inputValue;
            matrix.setValue(column, row, inputValue);
        }
    }
    return in;
}
Matrix& Matrix::operator=(const Matrix& matrix) {

    for (int column = 0; column < MAX_MATRIX_SIZE; ++column) {
        for (int row = 0; row < MAX_MATRIX_SIZE; ++row) {
            matrixArray[column][row] = matrix.getValue(column, row);
        }
    }
    return *this;
}
const Matrix Matrix::operator+(const Matrix& otherMatrix) const {
    return this->add(otherMatrix);
}
const Matrix Matrix::operator!() const {
    return this->transpose();
}

const Matrix Matrix::operator*(const Matrix& matrix) const
{
    return this->multi(matrix);
}

