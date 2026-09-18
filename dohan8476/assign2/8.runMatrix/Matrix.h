#pragma once;

class Matrix {
    public:
    Matrix();
    void read();
    void print();
    Matrix transpose();
    Matrix add(Matrix m);
    Matrix multi(Matrix m);
    int getMatrix(int row, int col);
    void setMatrix(int row, int col, int val);
    int getMaxWidth();
    void setMaxWidth(int maxWidth);

    private:
    const static int SIZE = 3;
    int matrix[SIZE][SIZE] = {};
    int maxWidth = 0;
};
