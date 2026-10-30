//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include <algorithm>
#include <cstddef>

#include "DebugAllocator.h"
#include "DefaultAllocator.h"


template<typename Data, typename Alloc = DefaultAllocator>
class RAIISharedPointer {
    Data *target;
    int *refCount;
public:

    RAIISharedPointer() : RAIISharedPointer(nullptr){
    }

    RAIISharedPointer(std::nullptr_t){
        target = nullptr;
        refCount = nullptr;
    }

    RAIISharedPointer(Data target) {
        this->target = Alloc::template allocate<Data>(std::move(target));
        this->refCount = Alloc::template allocate<int>(1);
    }

    ~RAIISharedPointer() {
        if (!refCount) return;
        if (--*refCount == 0) {
            Alloc::template deallocate<Data>(target);
            Alloc::template deallocate<int>(refCount);
        }
    }

    RAIISharedPointer(const RAIISharedPointer &other) {
        target = other.target;
        refCount = other.refCount;
        if (refCount) ++*refCount;
    }

    RAIISharedPointer & operator=(const RAIISharedPointer &other) {
        if (this == &other) return *this;
        if (refCount && --*refCount == 0) {
            Alloc::template deallocate<Data>(target);
            Alloc::template deallocate<int>(refCount);
        }

        target = other.target;
        refCount = other.refCount;
        if (refCount) ++*refCount;
        return *this;
    }

    RAIISharedPointer(RAIISharedPointer &&other) noexcept {
        target = nullptr;
        refCount = nullptr;
        std::swap(target, other.target);
        std::swap(refCount, other.refCount);
    }

    RAIISharedPointer &operator=(RAIISharedPointer &&other) noexcept {
        if (this == &other) return *this;
        std::swap(target, other.target);
        std::swap(refCount, other.refCount);
        return *this;
    }

    operator bool() const {
        return target != nullptr;
    }

    bool operator==(const RAIISharedPointer &other) const {
        return target == other.target;
    }

    Data & operator*() const {
        return *target;
    }
    [[nodiscard]] Data *operator->() const {
        return target;
    }

    Data * get() const {
        return target;
    }
};


