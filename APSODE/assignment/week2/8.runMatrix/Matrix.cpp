//
// Created by leegu on 26. 9. 17..
//

#include <iostream>
#include "Matrix.h"
using namespace std;

int get_digit_length(int target) {
    int count = 0;
    for (; target != 0; target /= 10) ++count;
    return count;
}

int Matrix::get_max_element_length() const {
    int max_length_element = 0;
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            int current_element = this->matrix[row][col] < 0 ? this->matrix[row][col] * -10 : this->matrix[row][col];
            if (max_length_element < current_element) {
                max_length_element = current_element;
            }
        }
    }

    return get_digit_length(max_length_element);
}


void Matrix::read() {
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            std::cin >> matrix[row][col];
        }
    }
}

void Matrix::print() const {
    int max_element_length = this->get_max_element_length();

    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        std::cout << "|";
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            printf(" %*d", max_element_length, matrix[row][col]);
        }
        std::cout << " |\n";
    }
}

Matrix Matrix::transpose() const {
    Matrix result_matrix = Matrix();
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            result_matrix.set(col, row, this->matrix[row][col]);
        }
    }

    return result_matrix;
}

Matrix Matrix::add(Matrix other_matrix) const {
    Matrix result_matrix = Matrix();
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            result_matrix.set(
                row, col,
                this->matrix[row][col] + other_matrix.get(row, col)
            );
        }
    }

    return result_matrix;
}

Matrix Matrix::multi(Matrix other_matrix) const {
    Matrix result_matrix = Matrix();
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            for (int temp = 0; temp < DEFAULT_ROW_SIZE; ++temp) {
                result_matrix.set(
                    row, col,
                    result_matrix.get(row, col) + this->matrix[row][temp] * other_matrix.get(temp, col)
                );
            }
        }
    }

    return result_matrix;
}

int Matrix::get(const int row, const int col) const {
    return this->matrix[row][col];
}

void Matrix::set(const int row, const int col, const int value) {
    this->matrix[row][col] = value;
}