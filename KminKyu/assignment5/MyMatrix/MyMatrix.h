#pragma once
#include <iostream>
using namespace std;

class MyMatrix {
    int maxColumn = 0;

    int maxRow = 0;

    int **matrixArray;

public:
    MyMatrix();

    MyMatrix(int Row, int column);

    MyMatrix(const MyMatrix& matrix);

    ~MyMatrix();

    MyMatrix& operator =(const MyMatrix& matrix);

    void init(const int*);

    const MyMatrix operator !() const;

    const MyMatrix operator +(const MyMatrix& matrix) const;

    const MyMatrix operator *(const MyMatrix& matrix) const;

    const MyMatrix transpose() const;

    const MyMatrix add(const MyMatrix& otherMatrix) const;

    const MyMatrix multi(const MyMatrix& otherMatrix) const;

    int getColumnLength(int column) const;

    int getMaxColumn() const;

    int getMaxRow() const;

    int getValue(int Row, int column) const;

    void setValue(int Row, int column, int value);

private:

    constexpr static int FAIL_VALUE = -999;

    static int getLength(int);

    static const MyMatrix failMatrix();
};

ostream& operator <<(ostream&, const MyMatrix&);

istream& operator >>(istream&, MyMatrix&);