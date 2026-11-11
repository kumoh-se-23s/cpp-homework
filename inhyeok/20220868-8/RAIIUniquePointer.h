//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include <algorithm>
#include <cstddef>

#include "DebugAllocator.h"
#include "DefaultAllocator.h"
#include "MyString.h"

template<typename Data, typename Alloc = DefaultAllocator>
class RAIIUniquePointer {
    Data *target;

public:
    RAIIUniquePointer() : RAIIUniquePointer(nullptr) {
    }

    RAIIUniquePointer(std::nullptr_t) {
        target = nullptr;
    }

    template<typename Child> requires std::is_base_of_v<Data, Child>
    RAIIUniquePointer(Child target) {
        this->target = Alloc::template allocate<Child>(std::move(target));
    }

    ~RAIIUniquePointer() {
        Alloc::template deallocate<Data>(target);
    }

    RAIIUniquePointer(const RAIIUniquePointer &other) = delete;

    RAIIUniquePointer &operator=(const RAIIUniquePointer &other) = delete;

    RAIIUniquePointer(RAIIUniquePointer &&other) noexcept {
        target = nullptr;
        std::swap(target, other.target);
    }

    RAIIUniquePointer &operator=(RAIIUniquePointer &&other) noexcept {
        if (this == &other) return *this;
        std::swap(target, other.target);
        return *this;
    }

    operator bool() const {
        return target != nullptr;
    }

    Data &operator*() const {
        return *target;
    }

    [[nodiscard]] Data *operator->() const {
        return target;
    }

    Data *get() const {
        return target;
    }

    [[nodiscard]] MyString toString() const {
        return target->toString();
    }

};

template<typename Data, typename Alloc>
std::ostream &operator<<(std::ostream &out, RAIIUniquePointer<Data, Alloc> &ptr) {
    return out << *ptr;
}
