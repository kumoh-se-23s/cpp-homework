#include<iostream>
#include <iomanip>
#include "MyMatrix.h"

class Matrix;
using namespace std;

MyMatrix::MyMatrix() {
    this->maxRow = 0 ;
    this->maxColumn = 0 ;
    this->matrixArray = nullptr;
};

MyMatrix::MyMatrix(int row, int column) {
    this->maxRow = row;
    this->maxColumn = column;
    this->matrixArray = new int*[row];
    for (int nowRow = 0; nowRow < row; ++nowRow) {
        this->matrixArray[nowRow] = new int[column];
        for (int nowColumn = 0; nowColumn < column; ++nowColumn) {
            this->matrixArray[nowRow][nowColumn] = 0;
        }
    }
}

MyMatrix::MyMatrix(const MyMatrix& matrix) {
    this->maxRow = matrix.maxRow;
    this->maxColumn = matrix.maxColumn;
    this->matrixArray = new int*[matrix.maxRow];
    for (int nowRow = 0; nowRow < matrix.maxRow; ++nowRow) {
        this->matrixArray[nowRow] = new int[matrix.maxColumn];
        for (int nowColumn = 0; nowColumn < matrix.maxColumn; ++nowColumn) {
            this->matrixArray[nowRow][nowColumn] = matrix.matrixArray[nowRow][nowColumn];
        }
    }
}

MyMatrix::~MyMatrix() {
    for (int row = 0; row < maxRow; ++row) {
        delete[] matrixArray[row];
    }
    delete[] matrixArray;
    matrixArray = nullptr;
}

void MyMatrix::init(const int* initArray) {
    for (int row = 0; row < maxRow; ++row) {
        for (int column = 0; column < maxColumn; ++column) {
            matrixArray[row][column] = initArray[row * maxColumn + column];
        }
    }
}

int MyMatrix::getMaxColumn() const {
    return maxColumn;
}

int MyMatrix::getMaxRow() const {
    return maxRow;
}

int MyMatrix::getLength(int number) {
    int cnt = (number < 0) ? 1 : 0;
    for (; number != 0; number /= 10) {
        ++cnt;
    }
    return cnt;
}

const MyMatrix MyMatrix::failMatrix() {
    MyMatrix failMatrix(1,1);
    failMatrix.setValue(0,0,FAIL_VALUE);
    return failMatrix;
}

int MyMatrix::getColumnLength(int column) const {
    int maxLength = 0;
    if (0 <= column && column < maxColumn) {
        for (int row = 0; row < maxRow; ++row) {
            int nowLength = getLength(matrixArray[row][column]);
            maxLength = maxLength > nowLength ? maxLength : nowLength;
        }
        return maxLength;
    }

    return getLength(FAIL_VALUE);
}

int MyMatrix::getValue(int row, int column) const {
    if (0 <= row && row < maxRow && 0 <= column && column < maxColumn) {
        return matrixArray[row][column];
    }
    return FAIL_VALUE;
}

void MyMatrix::setValue(int row, int column, int value) {
    if (0 <= row && row < maxRow && 0 <= column && column < maxColumn) {
        matrixArray[row][column] = value;
    }
}


const MyMatrix MyMatrix::transpose() const {
    MyMatrix resultMatrix(maxColumn, maxRow);
    for (int row = 0; row < maxRow; ++row) {
        for (int column = 0; column < maxColumn; ++column) {
            resultMatrix.matrixArray[column][row] = matrixArray[row][column];
        }
    }
    return resultMatrix;
}

const MyMatrix MyMatrix::add(const MyMatrix& otherMatrix) const {
    if (this->maxRow != otherMatrix.maxRow || this->maxColumn != otherMatrix.maxColumn) {
        return failMatrix();
    }

    MyMatrix resultMatrix(maxRow, maxColumn);

    for (int row = 0; row < maxRow; ++row) {
        for (int column = 0; column < maxColumn; ++column) {
            resultMatrix.matrixArray[row][column] =
                this->matrixArray[row][column] + otherMatrix.matrixArray[row][column];
        }
    }
    return resultMatrix;
}

const MyMatrix MyMatrix::multi(const MyMatrix& otherMatrix) const {

    if (this->maxColumn != otherMatrix.maxRow) {
        return failMatrix();
    }

    MyMatrix resultMatrix(this->maxRow, otherMatrix.maxColumn);
    for (int row = 0; row < this->maxRow; ++row) {
        for (int column = 0; column < otherMatrix.maxColumn; ++column) {

            for (int indexCount = 0; indexCount < this->maxColumn; ++indexCount) {
                resultMatrix.matrixArray[row][column] +=
                    this->matrixArray[row][indexCount] * otherMatrix.matrixArray[indexCount][column];
            }
        }
    }
    return resultMatrix;
}

MyMatrix& MyMatrix::operator =(const MyMatrix& otherMatrix) {
    if (&otherMatrix == this) {
        return *this;
    }

    int** array = new int*[otherMatrix.maxRow];
    for (int row = 0; row < otherMatrix.maxRow; ++row) {
        array[row] = new int[otherMatrix.maxColumn];
        for (int column = 0; column < otherMatrix.maxColumn; ++column) {
            array[row][column] = otherMatrix.matrixArray[row][column];
        }
    }
    for (int row = 0; row < maxRow; ++row) {
        delete[] matrixArray[row];
    }
    delete[] matrixArray;
    matrixArray = array;
    this->maxRow = otherMatrix.maxRow;
    this->maxColumn = otherMatrix.maxColumn;

    return *this;
}

const MyMatrix MyMatrix::operator +(const MyMatrix& MyMatrix) const {
    return this->add(MyMatrix);
}
const MyMatrix MyMatrix::operator !() const {
    return this->transpose();
}

const MyMatrix MyMatrix::operator *(const MyMatrix& matrix) const {
    return this->multi(matrix);
}

ostream& operator <<(ostream& out, const MyMatrix& matrix) {
    for (int row = 0; row < matrix.getMaxRow(); ++row) {
        for (int column = 0; column < matrix.getMaxColumn(); ++column) {

            if (column == 0) {
                out << "| ";
            }
            out << setw(matrix.getColumnLength(column)) << matrix.getValue(row, column);
            out << " ";
            if (column == matrix.getMaxColumn() - 1) {
                out << "|";
            }
        }
        out << "\n";
    }
    return out;
}

istream& operator >>(istream& in, MyMatrix& matrix){
    if (matrix.getMaxRow() <= 0 && matrix.getMaxColumn() <= 0) {
        return in;
    }
    int *inputValue = new int[matrix.getMaxRow() * matrix.getMaxColumn()];
    for (int row = 0; row < matrix.getMaxRow(); ++row) {
        for (int column = 0; column < matrix.getMaxColumn(); ++column) {
            in >> inputValue[row * matrix.getMaxColumn() + column];
        }
    }
    matrix.init(inputValue);
    delete[] inputValue;
    return in;
}