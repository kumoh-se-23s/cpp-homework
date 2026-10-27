#include "MyMatrix.h"
#include <iomanip>

// =================================================================
// PUBLIC SECTION
// =================================================================

MyMatrix::MyMatrix() {
    this->row = 1;
    this->col = 1;

    this->matrix = new int*[row];
    this->matrix[0] = new int[col];

    this->matrix[0][0] = -999;
}

MyMatrix::MyMatrix(int row, int col) {
    this->row = row;
    this->col = col;

    this->matrix = new int*[row];

    for (int i = 0; i < row; i++) {
        this->matrix[i] = new int[col];
    }

}
//복사 생성자
//덧셈, 곱셈에서 resultMatrix가 만들어지고 결과를 리턴하기 위한 임시 복사본 생성
//컴파일러는 임시 복사본을 얕은 복사본으로 만들고 add가 끝났으니 소멸자 호출로 원본 삭제
//그럼 임시 복사본도 원본이 없어지니 자기도 소멸자 호출 -> 값을 복사 받지 못하는 상황 발생
//복사 생성자를 통해 add등 함수가 끝났을 때 깊은 복사를 통해 값을 전달하게 만들어서 해결
MyMatrix::MyMatrix(const MyMatrix& m) {
    this->row = m.row;
    this->col = m.col;

    this->matrix = new int*[this->row];
    for (int i = 0; i < this->row; i++) {
        this->matrix[i] = new int[this->col];

        for (int j = 0; j < this->col; j++) {
            this->matrix[i][j] = m.matrix[i][j];
        }
    }
}

MyMatrix::~MyMatrix() {

    for (int i = 0; i < row; i++) {
        delete[] this->matrix[i];
    }

    delete[] this->matrix;
    matrix = nullptr;
}

void MyMatrix::init(const int *arr) const {
    int index = 0;
    for (int row = 0; row < this->row; row++) {
        for (int col = 0; col < this->col; col++) {
            this->matrix[row][col] = arr[index];
            index++;
        }
    }
}

int MyMatrix::getMaxWidth() const {
    int maxWidth = 0;
    for (int row = 0; row < this->row; row++) {
        for (int col = 0; col < this->col ; col++) {
            int currentWidth = getDigitWidth(matrix[row][col]);

            maxWidth = currentWidth > maxWidth ? currentWidth : maxWidth;
        }
    }
    return maxWidth;
}

int MyMatrix::getDigitWidth(int num) {
    int width = (num <= 0) ? 1 : 0;

    for (; num != 0; num /= 10, width++) {}

    return width;
}


int MyMatrix::getMatrix(const int row, const int col) const {
    if (0 <= row && row < this->row && 0 <= col && col < this->col) {
        return this->matrix[row][col];
    }
    //범위 밖은 -999 오류 출력
    return -999;
}

void MyMatrix::setMatrix(const int row, const int col, const int val) const {
    if (0 <= row && row < this->row && 0 <= col && col < this->col) {
        this->matrix[row][col] = val;
    }
}
int MyMatrix::getRow() const {
    return this->row;
}

int MyMatrix::getCol() const {
    return this->col;
}

// =================================================================
// OPERATOR OVERLOADING SECTION
// =================================================================

MyMatrix MyMatrix::operator +(const MyMatrix& m) const {
    return this->add(m);
}

MyMatrix MyMatrix::operator *(const MyMatrix& m) const {
    return this->multi(m);
}

MyMatrix& MyMatrix::operator =(const MyMatrix& m) {
    if (this == &m) {
        return *this;
    }

    for (int i = 0; i < this->row; i++) {
        delete[] this->matrix[i];
    }
    delete[] this->matrix;

    this->row = m.row;
    this->col = m.col;
    this->matrix = new int*[this->row];

    for (int i = 0; i < this->row; i++) {
        this->matrix[i] = new int[this->col];
        for (int j = 0; j < this->col; j++) {
            this->matrix[i][j] = m.matrix[i][j];
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const MyMatrix& m) {
    int maxWidth = m.getMaxWidth();

    for (int row = 0; row < m.getRow(); row++) {
        os << "|";
        for (int col = 0; col < m.getCol(); col++) {
            os << " " << std::setw(maxWidth) << m.getMatrix(row,col) << " ";
        }
        os << "|" << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, MyMatrix& m) {
    int val;
    for (int row = 0; row < m.getRow(); row++) {
        for (int col = 0; col < m.getCol(); col++) {
            is >> val;
            m.setMatrix(row, col, val);
        }
    }
    return is;
}

// =================================================================
// PRIVATE SECTION
// =================================================================

MyMatrix MyMatrix::add(const MyMatrix& m) const{
    //크기 다르면 합 불가
    if (this->row != m.row || this->col != m.col) {
        return MyMatrix();
    }

    MyMatrix resultMatrix(this->row, this->col);

    for (int row = 0; row < this->row; row++) {
        for (int col = 0; col < this->col; col++) {
            resultMatrix.matrix[row][col] = this->matrix[row][col] + m.matrix[row][col];
        }
    }
    return resultMatrix;
}

MyMatrix MyMatrix::multi(const MyMatrix& m) const{
    //곱은 this의 col과 m의 row가 같아야 곱 가능
    if (this->col != m.row) {
        return MyMatrix();
    }
    MyMatrix resultMatrix(this->row, m.col);

    for (int row = 0; row < this->row; row++) {
        for (int col = 0; col < m.col; col++) {
            resultMatrix.matrix[row][col] = 0;
            for (int cur = 0; cur < this->col; cur++) {
                resultMatrix.matrix[row][col] += matrix[row][cur] * m.matrix[cur][col];
            }
        }
    }
    return resultMatrix;
}