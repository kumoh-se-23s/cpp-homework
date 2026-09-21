#include <iostream>

using namespace std;

const int SIZE = 3;

//전치
void transpose(int matrix[][SIZE], int resultMatrix[][SIZE], int size) {
    for (int rowIdx = 0; rowIdx < size; rowIdx++) {
        for (int colIdx = 0; colIdx < size; colIdx++) {
            resultMatrix[colIdx][rowIdx] = matrix[rowIdx][colIdx];
        }
    }
}

//덧셈
void add(int matrix1[][SIZE], int matrix2[][SIZE], int resultMatrix[][SIZE], int size) {
    for (int rowIdx = 0; rowIdx < size; rowIdx++) {
        for (int colIdx = 0; colIdx < size; colIdx++) {
            resultMatrix[rowIdx][colIdx] = matrix1[rowIdx][colIdx] + matrix2[rowIdx][colIdx];
        }
    }
}

//곱셈
void multiply(int matrix1[][SIZE], int matrix2[][SIZE], int resultMatrix[][SIZE], int size) {
    for(int rowIdx = 0; rowIdx < size; rowIdx++) {
        for(int colIdx = 0; colIdx < size; colIdx++) {
            resultMatrix[rowIdx][colIdx] = 0;
            for(int idx = 0; idx < size; idx++) {
                resultMatrix[rowIdx][colIdx] += matrix1[rowIdx][idx] * matrix2[idx][colIdx];
            }
        }

    }
}

//출력
void printMatrix(int matrix[][SIZE],int size) {
    for (int rowIdx = 0; rowIdx < size; rowIdx++) {
        cout << "|";
        for (int colIdx = 0; colIdx < size; colIdx++) {
            printf("%3d",matrix[rowIdx][colIdx]);
        }
        cout << "|" << endl;
    }
}