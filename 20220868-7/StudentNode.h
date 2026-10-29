//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "RAIISharedStudentNode.h"
#include "Student.h"

struct StudentNode {
    Student data;

    RAIISharedStudentNode prevTeamMember = nullptr;
    RAIISharedStudentNode nextTeamMember = nullptr;

    RAIISharedStudentNode sameYearPrev = nullptr;
    RAIISharedStudentNode sameYearNext = nullptr;

    explicit StudentNode(Student data) : data(std::move(data)) {
    }
};
