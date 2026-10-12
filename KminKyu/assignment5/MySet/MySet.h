#pragma once
#include<iostream>

using namespace std;

class MySet {
public:
    MySet(int s = 0, int c = 4) : array(new int[c]), capacity(c), size(s) {}
    MySet(const MySet &mySet);
    ~MySet();
    MySet operator+(const MySet &mySet) const;
    MySet operator&(const MySet &mySet) const;
    MySet operator-(const MySet &mySet) const;
    MySet& operator=(const MySet &mySet);
    int getLength() const;
    int getCapacity() const;
    int get(int index) const;
    MySet unionSet(const MySet &mySet) const;
    MySet intersectionSet(const MySet &mySet) const;
    MySet differenceSet(const MySet &mySet) const;
    void append(int appendValue);
    void append(const MySet &mySet);
    void clear();

    void timSort();
    void mergeSort(int left, int mid, int right);
    void insertionSort(int left, int right);
    int binarySearch(int left, int right, int value);
    void nomarlize();
    void resize(int capacity);

private:
    int *array;
    int capacity;
    int size;
    constexpr static int MIN_RUN= 32;

};

istream& operator>>(istream& in, MySet& mySet);
ostream& operator<<(ostream& out, const MySet& mySet);
