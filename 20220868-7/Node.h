//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "RAIISharedPointer.h"


template<typename Data>
struct Node {
    Data data;
    RAIISharedPointer<Node> prev = nullptr;
    RAIISharedPointer<Node> next = nullptr;

    explicit Node(Data data) : data(std::move(data)) {
    }
};

template<>
struct Node<Student> {

    Student data;
    RAIISharedPointer<Node> prevTeamMember = nullptr;
    RAIISharedPointer<Node> nextTeamMember = nullptr;

    RAIISharedPointer<Node> sameYearPrev = nullptr;
    RAIISharedPointer<Node> sameYearNext = nullptr;

    explicit Node(Student data) : data(std::move(data)) {}
};
