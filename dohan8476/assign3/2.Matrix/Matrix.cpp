#include "Matrix.h"
#include <iomanip>

using namespace std;

Matrix :: Matrix() = default;

//연산자 오버로딩--------------

const Matrix Matrix::operator +(const Matrix& m) const {
    return this->add(m);
}

const Matrix Matrix::operator *(const Matrix& m) const {
    return this->multi(m);
}

const Matrix Matrix::operator!() const {
    return this->transpose();
}

ostream& operator<<(ostream& out, const Matrix& m) {
    int maxWidth = m.getMaxWidth();

    for (int row = 0; row < Matrix::SIZE; row++) {
        out << "|";
        for (int col = 0; col < Matrix::SIZE; col++) {
            out << " " << setw(maxWidth) << m.getMatrix(row,col) << " ";
        }
        out << "|" << endl;
    }
    return out;
}

istream& operator>>(istream& in, Matrix& m) {
    int inputVal;
    for (int row = 0; row < Matrix::SIZE; row++) {
        for (int col = 0; col < Matrix::SIZE; col++) {
            in >> inputVal;
            m.setMatrix(row,col,inputVal);
        }
    }

    return in;
}

//

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

int Matrix::getMatrix(int row, int col) const{
    return matrix[row][col];
}

void Matrix::setMatrix(int row, int col, int val) {
    matrix[row][col] = val;
}

Matrix Matrix::transpose() const{
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(col,row,this->getMatrix(row,col));
        }
    }

    return resultMatrix;
}

Matrix Matrix::add(Matrix m) const{
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(row,col, this->getMatrix(row,col) + m.getMatrix(row,col));
        }
    }

    return resultMatrix;
}

Matrix Matrix::multi(Matrix m) const{
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

