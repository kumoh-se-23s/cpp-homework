#pragma once

template<typename T>
int Comparator<T>::rowCompare(const T &a, const T &b) const {
    if (a == b) {
        return 0;
    }
    return 1;
}

template<typename T>
int Comparator<T>::columnCompare(const T &a, const T &b) const {
    auto result = a <=> b;

    if (result < 0) {
        return -1;
    }
    if (result > 0) {
        return 1;
    }
    return 0;
}
