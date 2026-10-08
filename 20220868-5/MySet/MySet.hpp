#pragma once
#include "MySet.h"
#include <iostream>

MySet::MySet(int capacity) : elements(capacity) {
}

MySet::~MySet() {
    // noop
}

MySet::MySet(const MySet &other) noexcept : elements(other.elements) {
}

MySet MySet::operator=(const MySet &other) noexcept {
    if (this == &other) return *this;
    elements = other.elements;
    return *this;
}

MySet::MySet(MySet &&other) noexcept : elements(std::move(other.elements)) {
}

MySet MySet::operator=(MySet &&other) noexcept {
    if (this == &other) return *this;
    elements = std::move(other.elements);
    return *this;
}

MySet MySet::operator&(const MySet &other) const {
    MySet set;
    for (int i = 0; i < elements.getSize(); ++i) {
        if (other.contains(elements[i]))
            set.add(elements[i]);
    }
    return set;
}

MySet MySet::operator+(const MySet &other) {
    MySet set;
    for (int i = 0; i < elements.getSize(); ++i) {
        set.add(elements[i]);
    }
    for (int i = 0; i < elements.getSize(); ++i) {
        set.add(other.elements[i]);
    }
    return set;
}

MySet MySet::operator-(const MySet &other) {
    MySet set;
    for (int i = 0; i < elements.getSize(); ++i) {
        for (int j = 0; j < elements.getSize(); ++j) {
            set.add(elements[i]);
        }
    }
    for (int i = 0; i < elements.getSize(); ++i) {
        for (int j = 0; j < elements.getSize(); ++j) {
            set.remove(other.elements[i]);
        }
    }
    return set;
}

int MySet::findPlacementIndex(const int value) const {
    int low = 0;
    int high = static_cast<int>(elements.getSize() - 1);
    if (high < 0) return ~0;
    while (low <= high) {
        const int mid = (low + high) / 2;
        const int v = elements[mid];
        if (v < value) {
            low = mid + 1;
        } else if (v > value) {
            high = mid - 1;
        } else {
            return mid;
        }
    }

    return ~low;
}

void MySet::add(int value) {
    int placementIndex = findPlacementIndex(value);
    if (placementIndex >= 0)
        return;
    placementIndex = ~placementIndex;
    elements.insert(placementIndex, value);
}

void MySet::remove(int value) {
    int placementIndex = findPlacementIndex(value);
    if (placementIndex < 0)
        return;
    elements.remove(placementIndex);
}

bool MySet::contains(int value) const {
    return findPlacementIndex(value) >= 0;
}

MyArray<int> &MySet::getData() {
    return elements;
}
const MyArray<int> &MySet::getData() const {
    return elements;
}

void MySet::clear() {
    elements.clear();
}
