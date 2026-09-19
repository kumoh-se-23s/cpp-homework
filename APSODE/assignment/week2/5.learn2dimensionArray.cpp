//
// Created by leegu on 26. 9. 16..
//


#include <iostream>
using namespace std ;
const int SIZE = 3 ; // 전역변수는 사용하지 않아야 하나, 이번 과제에서만 사용
void transpose(const int target_matrix[][SIZE], int result_matrix[][SIZE], const int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            result_matrix[target_col][target_row] = target_matrix[target_row][target_col];
        }
    }
}

void add(const int left_operand[][SIZE], const int right_operand[][SIZE], int result_matrix[][SIZE], const int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            result_matrix[target_row][target_col] = left_operand[target_row][target_col] + right_operand[target_row][target_col];
        }
    }
}

void multiply(const int left_operand[][SIZE], const int right_operand[][SIZE], int result_matrix[][SIZE], const int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            for (int temp = 0; temp < row_size; ++temp) {
                result_matrix[target_row][target_col] += left_operand[target_row][temp] * right_operand[temp][target_col];
            }
        }
    }
}

void printMatrix(const int matrix[][SIZE], const int row_size) {
    for (int target_row = 0; target_row < row_size; ++target_row) {
        std::cout << "|";
        for (int target_col = 0; target_col < SIZE; ++target_col) {
            std::cout << " " << matrix[target_row][target_col];
        }
        std::cout << " |\n";
    }
}