//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include <algorithm>
#include <cstddef>


template<typename Data>
class RAIISharedNode {
    Data *target;
    int *refCount;
public:

    RAIISharedNode() : RAIISharedNode(nullptr){
    }

    RAIISharedNode(std::nullptr_t){
        target = nullptr;
        refCount = new int(1);
    }

    RAIISharedNode(Data target) {
        this->target = new Data(std::move(target));
        this->refCount = new int(1);
    }

    ~RAIISharedNode() {
        if (--*refCount == 0) {
            delete target;
            delete refCount;
        }
    }

    RAIISharedNode(const RAIISharedNode &other) {
        target = other.target;
        refCount = other.refCount;
        ++*refCount;
    }

    RAIISharedNode & operator=(const RAIISharedNode &other) {
        if (this == &other) return *this;
        if (--*refCount == 0) {
            delete target;
        }

        target = other.target;
        refCount = other.refCount;
        ++*refCount;
        return *this;
    }

    RAIISharedNode(RAIISharedNode &&other) noexcept {
        target = nullptr;
        refCount = new int(1);
        std::swap(target, other.target);
        std::swap(refCount, other.refCount);
    }

    RAIISharedNode &operator=(RAIISharedNode &&other) noexcept {
        if (this == &other) return *this;
        std::swap(target, other.target);
        std::swap(refCount, other.refCount);
        return *this;
    }

    operator bool() const {
        return target != nullptr;
    }

    bool operator==(const RAIISharedNode &other) const {
        return target == other.target;
    }


    Data & operator*() const {
        return *target;
    }

    Data * get() const {
        return target;
    }
};


