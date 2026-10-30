//
// Created by Merutilm on 10/30/26.
//

#pragma once
#include <iostream>

struct DebugAllocator {

    template<typename T, typename... Args>
    static T * allocate(Args&&... args) {
        T * v = new T(std::forward<Args>(args)...);
        std::cout << "ALLOCATED " << v << std::endl;
        return v;
    }

    template<typename T>
    static void deallocate(T *ptr) {
        if (ptr == nullptr) return;
        std::cout << "DELETED " << ptr << std::endl;
        delete ptr;
    }
};
