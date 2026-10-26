#pragma once
#include <iosfwd>

class MyMatrix {
    public:
        MyMatrix();
        MyMatrix(int row, int col);
        MyMatrix(const MyMatrix& m);
        ~MyMatrix();

        void init(const int *arr) const;

        int getMatrix(int row, int col) const;
        void setMatrix(int row, int col, int val) const;

        int getMaxWidth() const;

        int getRow() const;
        int getCol() const;


        MyMatrix operator +(const MyMatrix & matrix) const;
        MyMatrix operator *(const MyMatrix & matrix) const;
        MyMatrix& operator =(const MyMatrix & matrix);

    private:
        int row = 0;
        int col = 0;
        int** matrix = nullptr;

        MyMatrix add(const MyMatrix& m) const;
        MyMatrix multi(const MyMatrix& m) const;

        static int getDigitWidth(int num);
};

std::ostream& operator<<(std::ostream& os, const MyMatrix& m);
std::istream& operator>>(std::istream& is, MyMatrix& m);