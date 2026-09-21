#pragma once

class MyArray2 {
public:
    MyArray2();
    void print();
    void setArray(int val, int index);

private:
    const static int SIZE = 10;
    int arr[SIZE] = {};
};

class MyArray1 {
public:
    bool readAscSorted();
    MyArray2 merge(MyArray1 arr);
    int getArray(int index);

private:
    const static int SIZE = 5;
    int arr[SIZE] = {};
};
