#pragma once

#include "Student.h"

struct StudentNode {
    Student student;
    //singly
    StudentNode* next;
    //circular
    StudentNode* nextTeam;
    StudentNode* prevTeam;

    StudentNode(const Student& student):student(student), next(nullptr), nextTeam(nullptr). prevTeam(nullptr) {};
    ~StudentNode() {}

    //student < overloading compared by stuID
    bool operator<(const StudentNode& other) const {
        return this->student < other.student;
    }
};
