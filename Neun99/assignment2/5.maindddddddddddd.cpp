#include <iostream>

using namespace std ;

const int SIZE = 3 ; // 전역변수는 사용하지 않아야 하나, 이번 과제에서만 사용

void transpose(int [][SIZE], int [][SIZE], int) ;
void add(int[][SIZE], int[][SIZE], int[][SIZE], int) ;
void multiply(int [][SIZE], int[][SIZE], int [][SIZE], int) ;
void printMatrix(int [][SIZE], int) ;

int main() {
    int matrix1[SIZE][SIZE] = {{1,2,3},{4,5,6},{7,8,9}}, matrix2[SIZE] [SIZE] = {{1,-1,0},{0,-1,1},{-1,1,0}} ;
    int transMatrix[SIZE][SIZE], sumMatrix[SIZE][SIZE], multiMatrix[SIZE][SIZE] ;
    transpose(matrix1, transMatrix, SIZE) ;
    cout << "Transpose\n" ;
    printMatrix(transMatrix, SIZE) ;
    add(matrix1, matrix2, sumMatrix, SIZE) ;
    cout << "Sum\n" ; printMatrix(sumMatrix, SIZE) ;
    multiply(matrix1, matrix2, multiMatrix, SIZE) ;
    cout << "Mutiplication\n" ; printMatrix(multiMatrix, SIZE) ;
return 0 ;
}

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