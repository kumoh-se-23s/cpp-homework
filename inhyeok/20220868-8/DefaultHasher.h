//
// Created by Merutilm on 11/10/26.
//

#pragma once
#include <cstddef>
template<typename T>
struct DefaultHasher {
    static size_t hash(T value) {
        return static_cast<size_t>(value);
    }
};