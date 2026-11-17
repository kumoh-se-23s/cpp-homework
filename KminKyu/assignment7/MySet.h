#pragma once
#include<iostream>
#include "KeyList.h"
#include "Student.h"
using namespace std;

template <typename T>
class MySet {
    using K = typename T::KeyType;
    T *array;


    int capacity;

    int size;

    constexpr static int DEFAULT_CAPACITY = 4;

public:

    MySet(int c = DEFAULT_CAPACITY) : array(new KeyList<K,V>[max(c, 0)]), capacity(max(c, 0)), size(0) {}

    MySet(const MySet<K, V> &otherSet);

    MySet(MySet<K, V> &&otherSet) noexcept;

    ~MySet();

    MySet& operator=(const MySet<K, V> &otherSet);

    MySet &operator=(MySet<K, V> &&otherSet) noexcept;

    int getLength() const;

    int getCapacity() const;

    T get(int index) const;

    void clear();

    void insert(const V& insertValue);

private:
    void resize();

    void resize(int capacity);

    static int calcInitialCapacity(int arraySize);

    void append(const V& appendValue);

    void append(const MySet<K, V> &otherSet);

    int binarySearch(int left, int right, const T& value) const;

};

template <typename T>
istream& operator>>(istream& in, MySet<T>& mySet);

template <typename T>
ostream& operator<<(ostream& out, const MySet<T>& mySet);
