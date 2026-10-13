#pragma once
#include <iostream>
using namespace std;

class MyMatrix {
public:
    MyMatrix();
    MyMatrix(int Row, int column);

    MyMatrix(const MyMatrix& matrix);

    ~MyMatrix();

    void init(const int*);

    const MyMatrix transpose() const;

    const MyMatrix add(const MyMatrix& otherMatrix) const;

    const MyMatrix multi(const MyMatrix& otherMatrix) const;

    int getMaxColumn() const;

    int getMaxRow() const;

    int getValue(int Row, int column) const;

    void setValue(int Row, int column, int value);

    const MyMatrix operator !() const;

    const MyMatrix operator +(const MyMatrix& matrix) const;

    const MyMatrix operator *(const MyMatrix& matrix) const;

    MyMatrix& operator =(const MyMatrix& matrix);

    int getColumnLength(int column) const;
private:
    int maxColumn = 0;
    int maxRow = 0;
    int **matrixArray;

    constexpr static int FAIL_VALUE = -999;

    static int getLength(int);

    static const MyMatrix failMatrix();
};

ostream& operator <<(ostream&, const MyMatrix&);
istream& operator >>(istream&, MyMatrix&);