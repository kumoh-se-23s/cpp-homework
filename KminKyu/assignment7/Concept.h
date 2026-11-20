// Concepts.h
#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <concepts>

#include "MyString.h"

template <typename T>
concept HasToString = requires(const T &t) {
    { t.toString() } -> std::convertible_to<MyString>;
};

#endif