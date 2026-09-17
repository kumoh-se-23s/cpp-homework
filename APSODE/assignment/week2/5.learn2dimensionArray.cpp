//
// Created by leegu on 26. 9. 16..
//


#include <iostream>
using namespace std ;
const int SIZE = 3 ; // 전역변수는 사용하지 않아야 하나, 이번 과제에서만 사용
void transpose(int target_matrix[][SIZE], int result_matrix[][SIZE], int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            result_matrix[target_col][target_row] = target_matrix[target_row][target_col];
        }
    }
}

void add(int left_operand[][SIZE], int right_operand[][SIZE], int result_matrix[][SIZE], int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            result_matrix[target_row][target_col] = left_operand[target_row][target_col] + right_operand[target_row][target_col];
        }
    }
}

void multiply(int left_operand[][SIZE], int right_operand[][SIZE], int result_matrix[][SIZE], int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            for (int temp = 0; temp < row_size; ++temp) {
                result_matrix[target_row][target_col] += left_operand[target_row][temp] * right_operand[temp][target_col];
            }
        }
    }
}

void printMatrix(int matrix[][SIZE], int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        std::cout << "|";
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            std::cout << " " << matrix[target_row][target_col];
        }
        std::cout << " |\n";
    }
}

int main() {
    int matrix1[SIZE][SIZE] = {{1,2,3},{4,5,6},{7,8,9}};
    int matrix2[SIZE][SIZE] = {{1,-1,0},{0,-1,1},{-1,1,0}} ;

    int transMatrix[SIZE][SIZE], sumMatrix[SIZE][SIZE], multiMatrix[SIZE][SIZE] = {0};

    // 전치
    transpose(matrix1, transMatrix, SIZE);
    cout << "Transpose\n";
    printMatrix(transMatrix, SIZE);

    // 행렬 덧셈
    add(matrix1, matrix2, sumMatrix, SIZE);
    cout << "Sum\n";
    printMatrix(sumMatrix, SIZE);

    // 행렬곱
    multiply(matrix1, matrix2, multiMatrix, SIZE);
    cout << "Mutiplication\n";
    printMatrix(multiMatrix, SIZE);

    return 0 ;
}