#pragma once

#include "Student.h"

struct StudentNode {
    Student data;
    //singly
    StudentNode* next;
    //circular
    StudentNode* nextTeam;
    StudentNode* prevTeam;

    StudentNode(const Student& std)
        : data(std), next(nullptr), prevTeam(nullptr), nextTeam(nullptr) {}

    ~StudentNode() {}

    //Student에서 학번 비교로 오버로딩
    bool operator<(const StudentNode& other) const {
        return this->data < other.data;
    }

    //Student 오버로딩 == => StudentId랑 비교
    bool operator==(int searchId) const {
        return this->data == searchId;
    }

    bool operator==(const StudentNode& other) const {
        return this->data == other.data;
    }
};
