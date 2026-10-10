#pragma once

class MyMatrix {
    public:
        MyMatrix();
        MyMatrix(int row, int col);
        ~MyMatrix();

        MyMatrix operator +(const MyMatrix & matrix) const;
        MyMatrix operator *(const MyMatrix & matrix) const;
        MyMatrix& operator =(const MyMatrix & matrix);

    private:
        int row = 0;
        int col = 0;
        int** matrix = nullptr;

        MyMatrix add(const MyMatrix& m) const;
        MyMatrix multi(const MyMatrix& m) const;

};