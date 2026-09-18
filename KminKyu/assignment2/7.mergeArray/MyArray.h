#pragma once

class MyArray2{
public:
    MyArray2();
    void print() const;
    int getNowSize() const;
    void append(int item);
    int get(int index) const;


private:
    static const int MAX_ARRAY_SIZE = 10;
    int array[MAX_ARRAY_SIZE] = {};
    int nowSize;    
};

class MyArray1{
public:
    MyArray1();
    bool readAscSorted();
    int getNowSize() const;
    int get(int index) const;
    MyArray2 merge(const MyArray1& mergeableArray) const;
private:
    static const int MAX_ARRAY_SIZE = 5;
    int array[MAX_ARRAY_SIZE] = {};
    int nowSize;    
};

