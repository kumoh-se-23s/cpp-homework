//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "RAIISharedStudentNode.h"
#include "Student.h"

template<typename Data, typename RAIINode>
struct Node {
    Data data;

    RAIINode prevTeamMember = nullptr;
    RAIINode nextTeamMember = nullptr;

    RAIINode sameYearPrev = nullptr;
    RAIINode sameYearNext = nullptr;

    explicit Node(Data data) : data(std::move(data)) {
    }
};

using StudentNode = Node<Student, RAIISharedStudentNode>;
