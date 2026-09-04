//
// Created by apsode on 26. 9. 4..
//

#ifndef CPP_HOMEWORK_PTRUTIL_H
#define CPP_HOMEWORK_PTRUTIL_H

#include <utility>

template <typename T>
class ScopedPointer{
private:
    T *target;

public:
    // UB 방지용 explicit 키워드
    explicit ScopedPointer(T *target) : target(target) {}

    ~ScopedPointer() {
        delete this->target;
    }

    T* get() const {
        return this->target;
    }

    T& operator*() const {
        return *this->target;
    }

    T* operator->() const {
        return this->target;
    }

    // 복사 & 대입의 금지
    // 얘도 UB를 막기 위함 -> 왜 이렇게 UB를 많이 하지?
    ScopedPointer(const ScopedPointer&) = delete;
    ScopedPointer& operator=(const ScopedPointer&) = delete;
};

template <typename T>
class ReferenceCountedPointer {
private:
    T *target;
    int *refCount;

    void release() {
        (*refCount)--;
        if (*refCount <= 0) {
            delete this->target;
            delete this->refCount;
        }
    }

public:
    // UB 방지용 explicit 키워드
    explicit ReferenceCountedPointer(T *target)
        : target(target), refCount(new int(1)) {
    }


    ReferenceCountedPointer(const ReferenceCountedPointer &other)
        : target(other.target), refCount(other.refCount) {
        (*refCount)++;
    }

    ReferenceCountedPointer& operator=(const ReferenceCountedPointer& other) {
        if (this == &other) {
            return *this;
        }

        this->release();

        target = other.target;
        refCount = other.refCount;
        (*refCount)++;

        return *this;
    }


    ~ReferenceCountedPointer() {
        this->release();
    }

    int useCount() const {
        return *this->refCount;
    }

    T* get() const {
        return this->target;
    }

    T& operator*() const {
        return *this->target;
    }

    T* operator->() const {
        return this->target;
    }

};

namespace ptr_util {
    template <typename T, typename... Args>
    ScopedPointer<T> make_scoped_pointer(Args&&... args) {
        return ScopedPointer<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args>
    ReferenceCountedPointer<T> make_counted_pointer(Args &&... args) {
        return ReferenceCountedPointer<T>(new T(std::forward<Args>(args)...));
    }
} // ptr_util

#endif //CPP_HOMEWORK_PTRUTIL_H
