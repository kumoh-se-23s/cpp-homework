//
// Created by Merutilm on 10/30/26.
//

#pragma once
#include <utility>

struct DefaultAllocator {

    template<typename T, typename... Args>
    static T * allocate(Args&&... args) {
        return new T(std::forward<Args>(args)...);
    }

    template<typename T>
    static void deallocate(T *ptr) {
        delete ptr;
    }
};
