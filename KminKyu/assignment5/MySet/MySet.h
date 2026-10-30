#pragma once
#include<iostream>

using namespace std;

class MySet {
    int *array;

    int capacity;

    int size;

    constexpr static int DEFAULT_CAPACITY = 4;

public:

    MySet(int c = DEFAULT_CAPACITY) : array(new int[max(c, 0)]), capacity(max(c, 0)), size(0) {}

    MySet(const MySet &otherSet);

    MySet(MySet &&otherSet) noexcept;

    ~MySet();

    MySet& operator=(const MySet &otherSet);

    MySet &operator=(MySet &&otherSet) noexcept;

    MySet operator+(const MySet &otherSet) const;

    MySet operator&(const MySet &otherSet) const;

    MySet operator-(const MySet &otherSet) const;

    int getLength() const;

    int getCapacity() const;

    int get(int index) const;

    MySet unionSet(const MySet &otherSet) const;

    MySet intersectionSet(const MySet &otherSet) const;

    MySet differenceSet(const MySet &otherSet) const;

    void clear();

    void insert(int insertValue);

    void resize();

private:
    void append(int appendValue);

    void append(const MySet &otherSet);

    int binarySearch(int left, int right, int value);
};

istream& operator>>(istream& in, MySet& mySet);

ostream& operator<<(ostream& out, const MySet& mySet);
