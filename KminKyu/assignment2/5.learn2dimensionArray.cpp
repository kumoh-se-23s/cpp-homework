#include <iostream>

using namespace std ;

const int SIZE = 3 ; // 전역변수는 사용하지 않아야 하나, 이번 과제에서만 사용

void transpose(int [][SIZE], int [][SIZE], int) ; void add(int[][SIZE], int[][SIZE], int[][SIZE], int) ;
void multiply(int [][SIZE], int[][SIZE], int [][SIZE], int) ; void printMatrix(int [][SIZE], int) ;

int main() {
    int matrix1[SIZE][SIZE] = {{1,2,3},{4,5,6},{7,8,9}}, matrix2[SIZE] [SIZE] = {{1,-1,0},{0,-1,1},{-1,1,0}} ;
    int transMatrix[SIZE][SIZE], sumMatrix[SIZE][SIZE], multiMatrix[SIZE][SIZE] ;

    transpose(matrix1, transMatrix, SIZE) ; cout << "Transpose\n" ; printMatrix(transMatrix, SIZE) ;
    add(matrix1, matrix2, sumMatrix, SIZE) ; cout << "Sum\n" ; printMatrix(sumMatrix, SIZE) ;
    multiply(matrix1, matrix2, multiMatrix, SIZE) ; cout << "Mutiplication\n" ; printMatrix(multiMatrix, SIZE) ;
    return 0 ;
}

void transpose(int baseMatrix[][SIZE], int resultMatrix[][SIZE], int size) {
    for (int column = 0; column < size; ++column) {
        for (int row = 0; row < size; ++row) {
            resultMatrix[row][column] = baseMatrix[column][row];
        }
    }
}

void add(int firstMatrix[][SIZE], int secondMatrix[][SIZE], int resultMatrix[][SIZE], int size) {
    for (int column = 0; column < size; ++column) {
        for (int row = 0; row < size; ++row) {
            resultMatrix[column][row] = firstMatrix[column][row] + secondMatrix[column][row];
        }
    }
}

void multiply(int firstMatrix[][SIZE], int secondMatrix[][SIZE], int resultMatrix[][SIZE], int size) {
    for (int column = 0; column < size; ++column) {
        for (int row = 0; row < size; ++row) {
            resultMatrix[column][row] = 0;
            for (int indexCount = 0; indexCount < size; ++indexCount) {
                resultMatrix[column][row] += firstMatrix[column][indexCount] * secondMatrix[indexCount][row];
            }
        }
    }
}

void printMatrix(int matrix[][SIZE], int size) {
    for (int column = 0; column < size; ++column) {
        for (int row = 0; row < size; ++row) {
            if (row == 0) {
                cout << "| ";
            }
            printf("%2d", matrix[column][row]);
            if (row == size - 1) {
                cout << "|";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}
