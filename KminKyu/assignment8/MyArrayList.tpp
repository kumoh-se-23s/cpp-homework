#pragma once

#include "MyArrayList.h"


template<typename T>
MyArrayList<T>::MyArrayList(int capacity) {
    this->items = new T[capacity];
    this->size = 0;
    this->capacity = capacity;
}

template<typename T>
MyArrayList<T>::MyArrayList(const MyArrayList &other) {
    this->items = new T[other.capacity];
    this->size = other.size;

    for (int index = 0; index < other.size; ++index) {
        this->items[index] = other.items[index];
    }
}

template<typename T>
MyArrayList<T>::MyArrayList(MyArrayList &&other) noexcept {
    this->items = other.items;
    other.items = nullptr;

    this->size = other.size;
    other.size = 0;

    this->capacity = other.capacity;
    other.capacity = 0;
}

template<typename T>
MyArrayList<T>::~MyArrayList() {
    delete[] items;
}

template<typename T>
MyArrayList<T> & MyArrayList<T>::operator=(const MyArrayList &other) {
    if (this == &other) {
        return *this;
    }
    T *result;
    if (this->capacity < other.capacity) {
        result = new T[other.capacity];
        this->capacity = other.capacity;
    } else {
        result = this->items;
    }

    for (int i = 0; i < other.size; ++i) {
        this->items[i] = other.items[i];
    }
    this->size = other.size;

    if (result != this->items) {
        delete[] items;
        this->items = result;
    }

    return *this;
}

template<typename T>
MyArrayList<T> & MyArrayList<T>::operator=(MyArrayList &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    this->items = other.items;
    other.items = nullptr;

    this->size = other.size;
    other.size = 0;

    this->capacity = other.capacity;
    other.capacity = 0;

    return *this;
}

template<typename T>
const T& MyArrayList<T>::at(int pos) const {
    if (0 <= pos && pos < size) {
        return items[pos];
    } else {
        return T();
    }
}

template<typename T>
int MyArrayList<T>::getSize() const {
    return size;
}

template<typename T>
int MyArrayList<T>::getCapacity() const {
    return capacity;
}

template<typename T>
void MyArrayList<T>::append(const T &item) {
    if (size + 1 >= capacity) {
        resize();
    }
    this->items[size++] = item;
}

template<typename T>
void MyArrayList<T>::clear() {
    this->size = 0;
}

template<typename T>
void MyArrayList<T>::resize() {
    this->capacity = (capacity == 0) ? DEFAULT_CAPACITY : capacity * 2;
    T *result = new T[this->capacity];

    for (int index = 0; index < this->size; ++index) {
        result[index] = items[index];
    }
    delete[] items;
    this->items = result;

}

