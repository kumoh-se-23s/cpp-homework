// Concepts.h
#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <concepts>

#include "OOldMyString.h"

template <typename T>
concept HasToString = requires(const T &t) {
    { t.toString() } -> std::convertible_to<OOldMyString>;
};

#endif