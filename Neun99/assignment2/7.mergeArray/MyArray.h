#pragma once
class MyArray2 {
public:
    int length();
    int size();
    void append(int);
    int get(int);
    void print();
private:
    const int LENGTH = 10;
    int arr[10] = {};
    int usingSize = 0;
};

class MyArray1 {
public:
    int length();
    int size();
    void append(int);
    int get(int);
    bool readAscSorted();
    MyArray2 merge(MyArray1);
private:
    const int LENGTH = 5;
    int arr[5] = {};
    int usingSize = 0;
};

