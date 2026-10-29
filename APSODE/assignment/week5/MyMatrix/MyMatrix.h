//
// Created by apsode on 26. 10. 29..
//

#ifndef CPP_HOMEWORK_MYMATRIX_H
#define CPP_HOMEWORK_MYMATRIX_H
#include <iosfwd>

class MyMatrix {
    constexpr static int FAIL_VALUE = -999;
    constexpr static int DEFAULT_ROW_SIZE = 1;
    constexpr static int DEFAULT_COLUMN_SIZE = 1;

    int **container;
    int m_row;
    int m_column;

    public:
        MyMatrix();

        MyMatrix(int row, int col);

        MyMatrix(const MyMatrix &matrix);

        ~MyMatrix();


        int row() const;

        int column() const;

        void set(int row, int column, int value);

        int get(int row, int column) const;

        //문제 조건 가정으로 항상 맞는 사이즈의 배열이 들어옴.
        void init(const int *source);

        MyMatrix operator=(const MyMatrix &matrix);

        MyMatrix operator+(const MyMatrix &other) const;

        MyMatrix operator*(const MyMatrix &other) const;


        int get_max_element_length() const;

    private:
        static bool can_add(const MyMatrix &left_operand, const MyMatrix &right_operand);

        static bool can_multiply(const MyMatrix &left_operand, const MyMatrix &right_operand);

        static int** create_empty_container(int row_size, int column_size);

        static MyMatrix create_error_matrix();

        void copy_from_other(const MyMatrix& other);

        bool is_valid_pos(int row, int column) const;

        MyMatrix add(const MyMatrix &other) const;

        MyMatrix multi(const MyMatrix &other) const;

        void free_container();
};

std::ostream& operator <<(std::ostream &output_stream, const MyMatrix &matrix);

std::istream& operator >>(std::istream &input_stream, MyMatrix &matrix);

#endif //CPP_HOMEWORK_MYMATRIX_H
