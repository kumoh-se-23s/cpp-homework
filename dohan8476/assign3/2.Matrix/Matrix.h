#pragma once
#include <iosfwd>

class Matrix {
    public:
    const static int SIZE = 3;

    Matrix();

    const Matrix operator +(const Matrix& m) const;
    const Matrix operator !() const;
    const Matrix operator *(const Matrix& m) const;

    int getMatrix(int row, int col) const;
    void setMatrix(int row, int col, int val);

    int getMaxWidth() const;
    int getDigitWidth(int num) const;

    private:
    Matrix transpose() const;
    Matrix add(Matrix m) const;
    Matrix multi(Matrix m) const;

    int matrix[SIZE][SIZE] = {};
};

std::ostream& operator<<(std::ostream& os, const Matrix& m);
std::istream& operator>>(std::istream& is, Matrix& m);