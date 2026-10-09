#pragma once
#include <iosfwd>

class Matrix {
    public:
        const static int SIZE = 3;

        Matrix();

        Matrix operator +(const Matrix& m) const;
        Matrix operator !() const;
        Matrix operator *(const Matrix& m) const;

        int getMatrix(int row, int col) const;
        void setMatrix(int row, int col, int val);

        int getMaxWidth() const;
        int getDigitWidth(int num) const;

    private:
        int matrix[SIZE][SIZE] = {};

        //C++17부터 도입된 문법 nodiscard가 붙어있으면 해당 메소드를 사용했을 때 반환되는 값을
        //무시하고 사용 안하면 에러를 띄움
        //[[nodiscard("행렬 연산의 결과값을 변수에 저장해야 합니다.")]]처럼 이유를 적을 수 있음
        [[nodiscard]] Matrix transpose() const;
        [[nodiscard]] Matrix add(const Matrix& m) const;
        [[nodiscard]] Matrix multi(const Matrix& m) const;

};

std::ostream& operator<<(std::ostream& os, const Matrix& m);
std::istream& operator>>(std::istream& is, Matrix& m);