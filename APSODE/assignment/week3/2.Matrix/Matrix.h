//
// Created by leegu on 26. 9. 17..
//

#ifndef CPP_HOMEWORK_8_RUNMATRIX_HPP
#define CPP_HOMEWORK_8_RUNMATRIX_HPP

#include <iostream>

class Matrix {
    public:
        static constexpr int DEFAULT_ROW_SIZE = 3;
        static constexpr int DEFAULT_COL_SIZE = 3;

        Matrix() = default;

        ~Matrix() = default;

        Matrix transpose() const;

        Matrix add(const Matrix &other_matrix) const;

        Matrix multi(const Matrix &other_matrix) const;

        int get(int row, int col) const;

        void set(int row, int col, int value);

        Matrix operator!() const;

        Matrix operator+(const Matrix &other_matrix) const;

        Matrix operator*(const Matrix &other_matrix) const;

        int get_max_element_length() const;

    private:
        int matrix[DEFAULT_ROW_SIZE][DEFAULT_COL_SIZE] = {};
};

std::ostream &operator <<(std::ostream &output_stream, const Matrix &matrix);

std::istream &operator >>(std::istream &input_stream, Matrix &matrix);

#endif //CPP_HOMEWORK_8_RUNMATRIX_HPP
