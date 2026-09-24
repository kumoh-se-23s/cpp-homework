#pragma once

class MyArray2 {
public:
    MyArray2();
    void print();
    void append(int val);

private:
    const static int CAPACITY = 10;
    int arr[CAPACITY] = {};
    int size = 0;
};

class MyArray1 {
public:
    bool readAscSorted();
    MyArray2 merge(MyArray1 arr);
    int getArray(int index);

private:
    const static int CAPACITY = 5;
    int arr[CAPACITY] = {};
    int size = 0;
};
