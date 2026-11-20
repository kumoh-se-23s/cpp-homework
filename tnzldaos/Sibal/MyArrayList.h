#pragma once
#include <string>
using namespace std;

template<typename T>
class MyArrayList {
    int capacity = 8;
    int size = 0;

    T *arrayList = new T[capacity];

    void resize();

public:
    MyArrayList() = default;

    ~MyArrayList();

    void append(const T &element);

    int getSize() const;

    T &get(int index);

    void remove(const string &input);

    T *findObject(const string &input);
};

#include "MyArrayList.tpp"

