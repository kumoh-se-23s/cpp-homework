#pragma once
#include <iostream>

using namespace std;

class MySet {
public:
    MySet();

    //getter
    int getSize() const;
    int getValue(int pos) const;

    //기능
    void insert(int value);

    //연산자 오버로딩
    const MySet operator+(const MySet& set2) const;
    const MySet operator&(const MySet& set2) const;
    const MySet operator-(const MySet& set2) const;
    MySet operator=(const MySet& set2);

    ~MySet();
private:
    static constexpr int DEFAULT_CAPACITY = 4;
    int capacity;
    int size;
    int *arr;

    void resize();
    void copyArr(int *copy, const int *src, int srcSize);
    void append(int value);
};

ostream& operator<<(ostream& out, const MySet& set);
istream& operator>>(istream& in, MySet& set);