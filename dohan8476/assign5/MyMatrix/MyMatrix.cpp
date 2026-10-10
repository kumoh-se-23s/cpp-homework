#include "MyMatrix.h"

MyMatrix::MyMatrix() {
    this->row = 1;
    this->col = 1;

    this->matrix = new int*[row];
    this->matrix[0] = new int[col];

    this->matrix[0][0] = -999;
}

MyMatrix::MyMatrix(int row, int col) {
    this->row = row;
    this->col = col;

    this->matrix = new int*[row];

    for (int i = 0; i < row; i++) {
        this->matrix[i] = new int[col];
    }

}

MyMatrix::~MyMatrix() {

    for (int i = 0; i < row; i++) {
        delete[] this->matrix[i];
    }

    delete[] this->matrix;
    matrix = nullptr;
}