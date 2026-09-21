#include <iostream>
#include "Matrix.h"

using namespace std;

Matrix :: Matrix() {
    int Matrix[SIZE][SIZE] = {};
}

void Matrix::print() {
    maxWidth = calcMaxWidth(max);

    for (int row = 0; row < SIZE; row++) {
        cout << "|";
        for (int col = 0; col < SIZE; col++) {
            printf(" %*d ", maxWidth, matrix[row][col]);
        }
        cout << "|" << endl;
    }
}

void Matrix::read() {
    int num;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            cin >> num;
            matrix[row][col] = num;
            updateMax(num);
        }
    }
}

void Matrix::updateMax(int num) {
    if (num < 0) {
        // -1 은 width가 2가 되어야함, 양수로 바꾸며 자릿수 +1)
        num *= -10;
    }

    if (num > max) {
        max = num;
    }
}


int Matrix::calcMaxWidth(int num) {
    int cnt = 0;

    while (num != 0) {
        cnt++;
        num /= 10;
    }

    return cnt;
}

int Matrix::getMatrix(int row, int col) {
    return matrix[row][col];
}

void Matrix :: setMatrix(int row, int col, int val) {
    matrix[row][col] = val;
}

Matrix Matrix::transpose() {
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(col,row,this->getMatrix(row,col));
            resultMatrix.updateMax(resultMatrix.getMatrix(col,row));
        }
    }

    return resultMatrix;
}

Matrix Matrix::add(Matrix m) {
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(row,col, this->getMatrix(row,col) + m.getMatrix(row,col));
            resultMatrix.updateMax(resultMatrix.getMatrix(row,col));
        }
    }

    return resultMatrix;
}

Matrix Matrix::multi(Matrix m) {
    Matrix resultMatrix;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            resultMatrix.setMatrix(row,col,0);
            for (int cur = 0; cur < SIZE; cur++) {
                resultMatrix.setMatrix(row,col,
                    resultMatrix.getMatrix(row,col)+ this->getMatrix(row,cur)*m.getMatrix(cur,col));
                resultMatrix.updateMax(resultMatrix.getMatrix(row,col));
            }
        }
    }

    return resultMatrix;
}
