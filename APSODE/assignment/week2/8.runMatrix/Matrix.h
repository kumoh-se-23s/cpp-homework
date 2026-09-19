//
// Created by leegu on 26. 9. 17..
//

#ifndef CPP_HOMEWORK_8_RUNMATRIX_HPP
#define CPP_HOMEWORK_8_RUNMATRIX_HPP

class Matrix {
    public:
        Matrix() = default;
        ~Matrix() = default;

        void read();
        void print() const;

        Matrix transpose() const;
        Matrix add(Matrix other_matrix) const;
        Matrix multi(Matrix other_matrix) const;

        int get(int row, int col) const;
        void set(int row, int col, int value);

    private:
        static constexpr int DEFAULT_ROW_SIZE = 3;
        static constexpr int DEFAULT_COL_SIZE = 3;
        int matrix[DEFAULT_ROW_SIZE][DEFAULT_COL_SIZE] = {};

        int get_max_element_length() const;
};


#endif //CPP_HOMEWORK_8_RUNMATRIX_HPP

