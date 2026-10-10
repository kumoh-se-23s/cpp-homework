#pragma once
#include <iosfwd>

class MyMatrix {
    public:
        MyMatrix();
        MyMatrix(int row, int col);
        MyMatrix(const MyMatrix& m);
        ~MyMatrix();

        MyMatrix operator +(const MyMatrix & matrix) const;
        MyMatrix operator *(const MyMatrix & matrix) const;
        MyMatrix& operator =(const MyMatrix & matrix);

        int getMatrix(int row, int col) const;
        void setMatrix(int row, int col, int val);

        int getMaxWidth() const;
        int getDigitWidth(int num) const;

        int getRow() const;
        int getCol() const;

        void init(int* arr);
    private:
        int row = 0;
        int col = 0;
        int** matrix = nullptr;

        MyMatrix add(const MyMatrix& m) const;
        MyMatrix multi(const MyMatrix& m) const;

};

std::ostream& operator<<(std::ostream& os, const MyMatrix& m);