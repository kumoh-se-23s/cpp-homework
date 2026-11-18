#ifndef MYSET_H
#define MYSET_H

#include<iostream>
using namespace std;

template <typename T>
class MySet {
    using K = typename T::K;
    using L = typename T::L;
    using V = typename L::V;

    T *array;

    int capacity;

    int size;

    constexpr static int DEFAULT_CAPACITY = 4;

public:

    MySet(int c = DEFAULT_CAPACITY) : array(new KeyLinkedListArray<K,T>[max(c, 0)]), capacity(max(c, 0)), size(0) {}

    MySet(const MySet &otherSet);

    MySet(MySet &&otherSet) noexcept;

    ~MySet();

    MySet& operator=(const MySet &otherSet);

    MySet &operator=(MySet &&otherSet) noexcept;

    int getLength() const;

    int getCapacity() const;

    T get(int index) const;

    void clear();

    void insert(const T& insertValue);

private:
    void resize();

    void resize(int capacity);

    static int calcInitialCapacity(int arraySize);

    void append(const V& appendValue);

    void append(const MySet<T> &otherSet);

    int binarySearch(int left, int right, const T& value) const;

};

template <typename T>
istream& operator>>(istream& in, MySet<T>& mySet);

template <typename T>
ostream& operator<<(ostream& out, const MySet<T>& mySet);

#include "MySet.tpp"
#endif // MYSET_H
