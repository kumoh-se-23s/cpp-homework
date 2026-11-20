#pragma once

template<typename T>
class Comparator {
public:
    ~Comparator() = default;

    int rowCompare(const T &a, const T &b) const;

    int columnCompare(const T &a, const T &b) const;
};

#include "Comparator.tpp"
