#include "MyMatrix.h"
#include <iomanip>

MyMatrix::MyMatrix() {
    column = 0;
    row = 0;
    arr =nullptr;
}

MyMatrix::MyMatrix(const MyMatrix& myMatrix) {
    arr = new int*[myMatrix.column];

    for(int index1 = 0 ; index1 < myMatrix.column ; index1++) {
        arr[index1] = new int[myMatrix.row];

        for(int index2 = 0 ; index2 < myMatrix.row ; index2++) {
            arr[index1][index2] = myMatrix.getElement(index1,index2);
        }
    }

    column = myMatrix.column;
    row = myMatrix.row;
}

MyMatrix::~MyMatrix() {
    for(int index = 0 ; index < column ; index++) {
        delete[] arr[index];
    }

    delete[] arr;
}

MyMatrix::MyMatrix(int column, int row) {
    this->column = column;
    this->row = row;

    arr = new int*[column];
    
    for(int index = 0 ; index < column ; index++) {
        //주소 선언
        arr[index] = new int[row];
    }
}

void MyMatrix::init(int* inputArr) {
    for(int index1 = 0 ; index1 < column ; index1++) {
        for(int index2 = 0 ; index2 < row ; index2++) {
            //실제 값 넣기
            arr[index1][index2] = inputArr[index1 * row + index2];
        }
    }
}


int MyMatrix::getColumn() const {
    return this->column;
}

void MyMatrix::setColumn(int column) {
    this->column = column;
}

int MyMatrix::getRow() const {
    return this->row;
}

void MyMatrix::setRow(int row) {
    this->row = row;
}

int MyMatrix::getElement(int column, int row) const {
    return arr[column][row];
}

MyMatrix MyMatrix::operator+(const MyMatrix& inputMyMatrix) const {
    MyMatrix resultMyMatrix;
    
    if(inputMyMatrix.column != column || inputMyMatrix.row != row) {
        resultMyMatrix = MyMatrix(1,1);

        resultMyMatrix.arr[0][0] = -999;

        return resultMyMatrix;
    }

    resultMyMatrix = MyMatrix(this->column, this->row);

    for(int index1 = 0; index1 < column ; index1++) {         
        for(int index2 = 0 ; index2 < row ; index2++) {
            resultMyMatrix.arr[index1][index2] = arr[index1][index2] + inputMyMatrix.arr[index1][index2];              
        }
    }

    return resultMyMatrix;
}

MyMatrix MyMatrix::operator*(const MyMatrix& inputMyMatrix) const {
    MyMatrix resultMyMatrix;
    
    if(row != inputMyMatrix.column) {
        resultMyMatrix = MyMatrix(1,1);
        resultMyMatrix.arr[0][0] = -999;

        return resultMyMatrix;
    }

    resultMyMatrix = MyMatrix(this->column,inputMyMatrix.row);
    for(int index1 = 0 ; index1 < column ; index1++) {
        for(int index2 = 0 ; index2 < inputMyMatrix.row ; index2++) {
            int sum = 0;

            for(int index3 = 0 ; index3 < row ; index3++) {
                sum += arr[index1][index3] * inputMyMatrix.arr[index3][index2];
            }

            resultMyMatrix.arr[index1][index2] = sum;
        }
    }

    return resultMyMatrix;
}

MyMatrix& MyMatrix::operator=(const MyMatrix& inputMyMatrix) {  
    if(this == &inputMyMatrix) {
        return *this;
    }
    
    for(int index = 0 ; index < column ; index++) {
        delete[] arr[index];
    }

    delete[] arr;

    column = 0;
    row = 0;

    arr = new int*[inputMyMatrix.column];
    
    for(int index = 0 ; index < inputMyMatrix.column ; index++) {
        //주소 선언
        arr[index] = new int[inputMyMatrix.row];
    }

    column = inputMyMatrix.column;
    row = inputMyMatrix.row;

    for(int index1 = 0 ; index1 < inputMyMatrix.column ; index1++) {
        for(int index2 = 0 ; index2 < inputMyMatrix.row ; index2++) {
            arr[index1][index2] = inputMyMatrix.arr[index1][index2];
        }
    }

    return *this;
}

ostream& operator<<(ostream& outputStream, const MyMatrix& myMatrix) {
    for(int index1 = 0 ; index1 < myMatrix.getColumn() ; index1++) {
        outputStream << "|";

        for(int index2 = 0 ; index2 < myMatrix.getRow() ; index2++) {
            outputStream << setw(4) << myMatrix.getElement(index1, index2);
        }
        outputStream << "|" << endl;
    }
    return outputStream;
}