#include <iostream>
#include "MyMatrix.h"

MyMatrix::MyMatrix(const int rows, const int cols): rows(rows), cols(cols) {
    data = new int *[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new int[cols];
    }
}

MyMatrix::~MyMatrix() {
    for (int i = 0; i < cols; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

MyMatrix::MyMatrix(const MyMatrix &other) noexcept: rows(other.rows), cols(other.cols) {
    data = new int *[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

MyMatrix & MyMatrix::operator=(const MyMatrix &other) noexcept {
    if (this == &other) return *this;

    if (other.rows != rows || other.cols != cols) {
        for (int i = 0; i < cols; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = new int *[other.rows];
        for (int i = 0; i < other.rows; ++i) {
            data[i] = new int[other.cols];
        }
    }
    rows = other.rows;
    cols = other.cols;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

MyMatrix::MyMatrix(MyMatrix &&other) noexcept {
    std::swap(data,other.data);
    std::swap(rows,other.rows);
    std::swap(cols,other.cols);
}

MyMatrix & MyMatrix::operator=(MyMatrix &&other) noexcept {
    if (this == &other) return *this;

    std::swap(data,other.data);
    std::swap(rows,other.rows);
    std::swap(cols,other.cols);
    return *this;
}

MyMatrix MyMatrix::operator!() const
{
    MyMatrix result(cols, rows);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}
MyMatrix MyMatrix::operator+(const MyMatrix &other) const
{
    MyMatrix result(rows, cols);
    if (other.rows != rows || other.cols != cols) return invalidMatrixSentinel();
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

MyMatrix MyMatrix::operator*(const MyMatrix &other) const
{
    if (other.rows != cols) return invalidMatrixSentinel();
    MyMatrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < other.cols; ++j)
        {
            int sum = 0;
            for (int k = 0; k < cols; ++k)
            {
                sum += data[i][k] * other.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

void MyMatrix::init(const int *arr) const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = arr[i * cols + j];
        }
    }
}

int MyMatrix::getLen(int v)
{
    int cnt = (v < 0);
    for (; v != 0; v /= 10)
    {
        ++cnt;
    }
    return cnt;
};


// int main() {
//     const int MAX_CNT = 8 ;
//     MyMatrix matArr[MAX_CNT] ;
//     matArr[0] = MyMatrix(2,2) ; // 행렬 크기 초기화
//     matArr[1] = MyMatrix(2,2) ;
//     matArr[2] = MyMatrix(2,3) ;
//     int arr0[] = {1,2,3,4}, arr1[] = {1, -1, 0, 2} ;
//     int arr2[] = {1,2,3,3,2,1} ; // 배열 내용 초기화
//     matArr[0].init(arr0) ; // arr0으로 시작하는 배열에
//     matArr[1].init(arr1) ; // 적절한 개수의 값이 있다고
//     matArr[2].init(arr2) ; // 가정하고 행렬값 행우선 초기화
//     matArr[3] = matArr[0] + matArr[1] ;
//     matArr[4] = matArr[0] + matArr[2] ;
//     matArr[5] = matArr[0] * matArr[1] ;
//     matArr[6] = matArr[2] * matArr[0] ;
//     matArr[7] = matArr[0] * matArr[2] ;
//     for (int i = 0 ; i < MAX_CNT ; i++) { // 출력
//         std::cout << "-- Matrix " << i << " --\n" ;
//         std::cout << matArr[i] << std::endl;
//     }
//     return 0;
// }