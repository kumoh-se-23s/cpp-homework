//
// Created by Merutilm on 10/28/26.
//

#include "RAIISharedStudentNode.h"
#include "Student.h"
#include "StudentNode.h"


RAIISharedStudentNode::RAIISharedStudentNode() : RAIISharedStudentNode(nullptr){
}

RAIISharedStudentNode::RAIISharedStudentNode(std::nullptr_t){
    target = nullptr;
    refCount = nullptr;
}

RAIISharedStudentNode::RAIISharedStudentNode(Student target) {
    this->target = new StudentNode(std::move(target));
    this->refCount = new int(1);
}

RAIISharedStudentNode::~RAIISharedStudentNode() {
    if (--*refCount == 0) {
        delete target;
        delete refCount;
    }
}

RAIISharedStudentNode::RAIISharedStudentNode(const RAIISharedStudentNode &other) {
    target = other.target;
    refCount = other.refCount;
    ++*refCount;
}

RAIISharedStudentNode & RAIISharedStudentNode::operator=(const RAIISharedStudentNode &other) {
    if (this == &other) return *this;
    if (--*refCount == 0) {
        delete target;
    }

    target = other.target;
    refCount = other.refCount;
    return *this;
}

RAIISharedStudentNode::RAIISharedStudentNode(RAIISharedStudentNode &&other) noexcept {
    target = nullptr;
    refCount = new int(1);
    std::swap(target, other.target);
    std::swap(refCount, other.refCount);
}

RAIISharedStudentNode &RAIISharedStudentNode::operator=(RAIISharedStudentNode &&other) noexcept {
    if (this == &other) return *this;
    std::swap(target, other.target);
    std::swap(refCount, other.refCount);
    return *this;
}

RAIISharedStudentNode::operator bool() const {
    return target != nullptr;
}

bool RAIISharedStudentNode::operator==(const RAIISharedStudentNode &other) const {
    return target == other.target;
}


StudentNode & RAIISharedStudentNode::operator*() const {
    return *target;
}

StudentNode * RAIISharedStudentNode::get() const {
    return target;
}
