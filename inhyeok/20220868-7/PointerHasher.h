//
// Created by Merutilm on 11/10/26.
//

#pragma once
#include <cstdint>

#include "RAIISharedPointer.h"

template<typename T>
struct PointerHasher {
    static int hash(const RAIISharedPointer<T> &value) {
        return reinterpret_cast<uintptr_t>(value.get());
    }
};
