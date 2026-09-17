#pragma once

class MyArray2{
public:
    MyArray2();
    bool readAscSorted();
    void print();
    int getNowSize() const;
    bool append(int item);
    int get(int index) const;


private:
    static const int MAX_ARRAY_SIZE = 10;
    int array[10] = {0,};
    int nowSize;    
};

class MyArray1{
public:
    MyArray1();
    bool readAscSorted();
    void print();
    int getNowSize() const;
    bool append(int item);
    int get(int index) const;
    MyArray2 merge(MyArray1 mergeableArray) const; //MyArray1에만 존재하는 메소드
private:
    static const int MAX_ARRAY_SIZE = 5;
    int array[5] = {0,};
    int nowSize;    
};

