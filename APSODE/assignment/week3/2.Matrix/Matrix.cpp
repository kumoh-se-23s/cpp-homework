//
// Created by leegu on 26. 9. 17..
//

#include <iostream>
#include "Matrix.h"


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

Matrix Matrix::transpose() const {
    Matrix result_matrix = Matrix();
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            result_matrix.matrix[col][row] = this->matrix[row][col];
        }
    }

    return result_matrix;
}

Matrix Matrix::add(const Matrix &other_matrix) const {
    Matrix result_matrix = Matrix();
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            result_matrix.matrix[row][col] = this->matrix[row][col] + other_matrix.matrix[row][col];
        }
    }

    return result_matrix;
}

Matrix Matrix::multi(const Matrix &other_matrix) const {
    Matrix result_matrix = Matrix();
    for (int row = 0; row < DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < DEFAULT_COL_SIZE; ++col) {
            for (int temp = 0; temp < DEFAULT_ROW_SIZE; ++temp) {
                result_matrix.matrix[row][col] += this->matrix[row][temp] * other_matrix.matrix[temp][col];
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

Matrix Matrix::operator!() const {
    return this->transpose();
}

Matrix Matrix::operator+(const Matrix &other_matrix) const {
    return this->add(other_matrix);
}

Matrix Matrix::operator*(const Matrix &other_matrix) const {
    return this->multi(other_matrix);
}

std::ostream &operator<<(std::ostream &output_stream, const Matrix &matrix) {
    int max_element_length = matrix.get_max_element_length();

    for (int row = 0; row < Matrix::DEFAULT_ROW_SIZE; ++row) {
        output_stream << "|";
        for (int col = 0; col < Matrix::DEFAULT_COL_SIZE; ++col) {
            printf(" %*d", max_element_length, matrix.get(row, col));
        }
        output_stream << " |\n";
    }

    return output_stream;
}

std::istream &operator>>(std::istream &input_stream, Matrix &matrix) {
    for (int row = 0; row < Matrix::DEFAULT_ROW_SIZE; ++row) {
        for (int col = 0; col < Matrix::DEFAULT_COL_SIZE; ++col) {
            int input_buffer;
            input_stream >> input_buffer;
            matrix.set(row, col, input_buffer);
        }
    }

    return input_stream;
}
