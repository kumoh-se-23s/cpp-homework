#include "Matrix.h"
#include <iomanip>

using namespace std;

Matrix :: Matrix() = default;

//연산자 오버로딩--------------

Matrix Matrix::operator +(const Matrix& m) const {
    return this->add(m);
}

Matrix Matrix::operator *(const Matrix& m) const {
    return this->multi(m);
}

Matrix Matrix::operator!() const {
    return this->transpose();
}

ostream& operator<<(ostream& os, const Matrix& m) {
    int maxWidth = m.getMaxWidth();

    for (int row = 0; row < Matrix::SIZE; row++) {
        os << "|";
        for (int col = 0; col < Matrix::SIZE; col++) {
            os << " " << setw(maxWidth) << m.getMatrix(row,col) << " ";
        }
        os << "|" << endl;
    }
    return os;
}

istream& operator>>(istream& is, Matrix& m) {
    int inputVal;
    for (int row = 0; row < Matrix::SIZE; row++) {
        for (int col = 0; col < Matrix::SIZE; col++) {
            is >> inputVal;
            m.setMatrix(row,col,inputVal);
        }
    }

    return is;
}

int Matrix::getMaxWidth() const{
    int maxWidth = 0;
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            int currentWidth = getDigitWidth(getMatrix(row, col));

            maxWidth = currentWidth > maxWidth ? currentWidth : maxWidth;
        }
    }
    return maxWidth;
}

int Matrix::getDigitWidth(int num) const{
    int width = (num <= 0) ? 1 : 0;

    for (; num != 0; num /= 10, width++) {}

    return width;
}

int Matrix::getMatrix(const int row, const int col) const{
    return matrix[row][col];
}

void Matrix::setMatrix(const int row, const int col, const int val) {
    if (0 <= row && row < SIZE && 0 <= col && col < SIZE) {
        matrix[row][col] = val;
    }
}

Matrix Matrix::transpose() const{
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.matrix[col][row] = matrix[row][col];
        }
    }

    return resultMatrix;
}

Matrix Matrix::add(const Matrix& m) const{
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.matrix[row][col] = this->matrix[row][col] + m.matrix[row][col];
        }
    }

    return resultMatrix;
}

Matrix Matrix::multi(const Matrix& m) const{
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.matrix[row][col] = 0;
            for (int cur = 0; cur < SIZE; cur++) {
                resultMatrix.matrix[row][col] += matrix[row][cur] * m.matrix[cur][col];
            }
        }
    }
    return resultMatrix;
}

