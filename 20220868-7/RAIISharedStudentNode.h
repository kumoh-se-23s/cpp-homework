//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include <cstddef>

#include "Student.h"

struct StudentNode;

class RAIISharedStudentNode {
    StudentNode *target;
    int *refCount;
public:

    RAIISharedStudentNode();

    RAIISharedStudentNode(std::nullptr_t);

    explicit RAIISharedStudentNode(Student target);

    ~RAIISharedStudentNode();

    RAIISharedStudentNode(const RAIISharedStudentNode &other);

    RAIISharedStudentNode &operator=(const RAIISharedStudentNode &other);

    RAIISharedStudentNode(RAIISharedStudentNode &&other) noexcept;

    RAIISharedStudentNode &operator=(RAIISharedStudentNode &&other) noexcept;

    operator bool() const;

    bool operator==(const RAIISharedStudentNode &other) const;

    [[nodiscard]] StudentNode &operator*() const;

    [[nodiscard]] StudentNode *operator->() const {
        return target;
    }

    [[nodiscard]] StudentNode *get() const;
};
