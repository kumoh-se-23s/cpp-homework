#include <iostream>
using namespace std ;

const int SIZE = 3; // 전역변수는 사용하지 않아야 하나, 이번 과제에서만 사용
void transpose(int [][SIZE], int [][SIZE], int);
void add(int [][SIZE], int [][SIZE], int [][SIZE], int);
void multiply(int [][SIZE], int [][SIZE], int [][SIZE], int);
void printMatrix(int [][SIZE], int);

int main() {
    int matrix1[SIZE][SIZE] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
    matrix2[SIZE][SIZE] = {{1, -1, 0}, {0, -1, 1}, {-1, 1, 0}};

    int transMatrix[SIZE][SIZE], sumMatrix[SIZE][SIZE], multiMatrix[SIZE][SIZE];

    transpose(matrix1, transMatrix, SIZE);
    cout << "Transpose\n";
    printMatrix(transMatrix, SIZE);

    add(matrix1, matrix2, sumMatrix, SIZE);
    cout << "Sum\n";
    printMatrix(sumMatrix, SIZE);

    multiply(matrix1, matrix2, multiMatrix, SIZE);
    cout << "Mutiplication\n";
    printMatrix(multiMatrix, SIZE);

    return 0;
}

void printMatrix(int inputMatrix[][SIZE], int size) {
    for (int i = 0; i < size; i++) {
        cout << "|";
        for (int j = 0; j < size; j++) {
            printf("%2d ", inputMatrix[i][j]);
        }
        cout << "|" << endl;
    }
}

void transpose(int inputMatrix[][SIZE], int resultMatrix[][SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = inputMatrix[j][i];
        }
    }
}

void add(int inputMatrix1[][SIZE], int inputMatrix2[][SIZE], int resultMatrix[][SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = inputMatrix1[i][j] + inputMatrix2[i][j];
        }
    }
}

void multiply(int inputMatrix1[][SIZE], int inputMatrix2[][SIZE], int resultMatrix[][SIZE], int size) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < size; k++) {
                resultMatrix[i][j] += inputMatrix1[i][k] * inputMatrix2[k][j];
            }
        }
    }
}
