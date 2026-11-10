//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "DoubleNode.h"
#include "RAIISharedPointer.h"
#include "Student.h"


struct StudentNode {

    Student data;
    RAIISharedPointer<StudentNode> prevTeamMember = nullptr;
    RAIISharedPointer<StudentNode> nextTeamMember = nullptr;

    RAIISharedPointer<StudentNode> sameYearNext = nullptr;

    StudentNode(Student data) : data(std::move(data)) {}
};
