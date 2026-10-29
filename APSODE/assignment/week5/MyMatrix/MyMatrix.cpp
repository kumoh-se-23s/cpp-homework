//
// Created by apsode on 26. 10. 29..
//

#include "MyMatrix.h"
#include <iomanip>

int get_digit_length(int target) {
    int count = 0;
    for (; target != 0; target /= 10) ++count;
    return count;
}

MyMatrix::MyMatrix()
    : m_row(DEFAULT_ROW_SIZE), m_column(DEFAULT_COLUMN_SIZE) {
    this->container = create_empty_container(DEFAULT_ROW_SIZE, DEFAULT_COLUMN_SIZE);
}

MyMatrix::MyMatrix(int row, int col)
    : m_row(row), m_column(col) {
    this->container = create_empty_container(row, col);
}

MyMatrix::MyMatrix(const MyMatrix &matrix)
    : m_row(matrix.m_row), m_column(matrix.m_column) {
    this->container = create_empty_container(matrix.m_row, matrix.m_column);
    this->copy_from_other(matrix);
}

MyMatrix::~MyMatrix() {
    this->free_container();
}

int MyMatrix::row() const {
    return this->m_row;
}

int MyMatrix::column() const {
    return this->m_column;
}

void MyMatrix::set(int row, int column, int value) {
    if (this->is_valid_pos(row, column)) {
        this->container[row][column] = value;
    }
}

int MyMatrix::get(int row, int column) const {
    if (this->is_valid_pos(row, column)) {
        return this->container[row][column];
    }

    return FAIL_VALUE;
}

void MyMatrix::init(const int *source) {
    for (int row_index = 0; row_index < this->m_row; ++row_index) {
        for (int column_index = 0; column_index < this->m_column; ++column_index) {
            this->container[row_index][column_index] = source[row_index * this->m_column + column_index];
        }
    }
}

MyMatrix& MyMatrix::operator=(const MyMatrix &matrix) {
    if (this == &matrix) {
        return *this;
    }

    MyMatrix temp_matrix = MyMatrix(matrix);
    swap(*this, temp_matrix);
    return *this;
}

MyMatrix MyMatrix::operator+(const MyMatrix &other) const {
    return this->add(other);
}

MyMatrix MyMatrix::operator*(const MyMatrix &other) const {
    return this->multi(other);
}

int** MyMatrix::create_empty_container(int row_size, int column_size) {
    int **new_container = new int *[row_size];

    for (int row_index = 0; row_index < row_size; ++row_index) {
        new_container[row_index] = new int[column_size]();
    }

    return new_container;
}

MyMatrix MyMatrix::create_error_matrix() {
    MyMatrix error_matrix = MyMatrix();
    error_matrix.container[0][0] = FAIL_VALUE;

    return error_matrix;
}

int MyMatrix::get_max_element_length() const {
    int max_length_element = 0;
    for (int row = 0; row < this->m_row; ++row) {
        for (int col = 0; col < this->m_column; ++col) {
            int current_element = this->container[row][col] < 0
                                      ? this->container[row][col] * -10
                                      : this->container[row][col];

            if (max_length_element < current_element) {
                max_length_element = current_element;
            }
        }
    }

    return get_digit_length(max_length_element);
}

bool MyMatrix::is_valid_pos(int row, int column) const {
    return 0 <= row && row < this->m_row && 0 <= column && column < this->m_column;
}

void MyMatrix::copy_from_other(const MyMatrix &other) {
    for (int row_index = 0; row_index < this->m_row; ++row_index) {
        for (int column_index = 0; column_index < this->m_column; ++column_index) {
            this->container[row_index][column_index] = other.container[row_index][column_index];
        }
    }
}

void MyMatrix::swap(MyMatrix &matrix1, MyMatrix &matrix2) {
    int **temp_container = matrix1.container;
    matrix1.container = matrix2.container;
    matrix2.container = temp_container;

    int temp_row = matrix1.m_row;
    matrix1.m_row = matrix2.m_row;
    matrix2.m_row = temp_row;

    int temp_column = matrix1.m_column;
    matrix1.m_column = matrix2.m_column;
    matrix2.m_column = temp_column;
}

void MyMatrix::free_container() {
    for (int row_index = 0; row_index < this->m_row; ++row_index) {
        delete[] this->container[row_index];
    }
    delete[] this->container;

    this->container = nullptr;
}

bool MyMatrix::can_add(const MyMatrix &left_operand, const MyMatrix &right_operand) {
    return left_operand.m_row == right_operand.m_row && left_operand.m_column == right_operand.m_column;
}

bool MyMatrix::can_multiply(const MyMatrix &left_operand, const MyMatrix &right_operand) {
    return left_operand.m_column == right_operand.m_row;
}

MyMatrix MyMatrix::add(const MyMatrix &other_matrix) const {
    if (!can_add(*this, other_matrix)) {
        return create_error_matrix();
    }

    MyMatrix result_matrix = MyMatrix(this->m_row, this->m_column);

    for (int row = 0; row < this->m_row; ++row) {
        for (int col = 0; col < this->m_column; ++col) {
            result_matrix.container[row][col] = this->container[row][col] + other_matrix.container[row][col];
        }
    }

    return result_matrix;
}

MyMatrix MyMatrix::multi(const MyMatrix &other_matrix) const {
    if (!can_multiply(*this, other_matrix)) {
        return create_error_matrix();
    }

    MyMatrix result_matrix = MyMatrix(this->m_row, other_matrix.m_column);

    for (int row = 0; row < this->m_row; ++row) {
        for (int col = 0; col < other_matrix.m_column; ++col) {
            for (int temp = 0; temp < this->m_column; ++temp) {
                result_matrix.container[row][col] += this->container[row][temp] * other_matrix.container[temp][col];
            }
        }
    }

    return result_matrix;
}


std::ostream& operator<<(std::ostream &output_stream, const MyMatrix &matrix) {
    int max_element_length = matrix.get_max_element_length();

    for (int row = 0; row < matrix.row(); ++row) {
        output_stream << "|";
        for (int col = 0; col < matrix.column(); ++col) {
            output_stream << " " << std::setw(max_element_length) << matrix.get(row, col);
        }
        output_stream << " |\n";
    }

    return output_stream;
}

std::istream& operator>>(std::istream &input_stream, MyMatrix &matrix) {
    for (int row = 0; row < matrix.row(); ++row) {
        for (int col = 0; col < matrix.column(); ++col) {
            int input_buffer;
            input_stream >> input_buffer;
            matrix.set(row, col, input_buffer);
        }
    }

    return input_stream;
}
