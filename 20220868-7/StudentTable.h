//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "Student.h"
#include <stop_token>

#include "GenericCircularLinkedList.h"
#include "KeyValuePair.h"
#include "StudentCircularLinkedList.h"
#include "TeamMemberCircularLinkedList.h"

class StudentHashTable {
    GenericCircularLinkedList<KeyValuePair<int, StudentCircularLinkedList>> studentTable;
    GenericCircularLinkedList<KeyValuePair<int, TeamMemberCircularLinkedList>> teamTable;

    constexpr static int INITIAL_CAPACITY = 16;

public:
    explicit StudentHashTable();

    ~StudentHashTable();

    StudentHashTable(const StudentHashTable &) = delete;

    StudentHashTable &operator=(const StudentHashTable &) = delete;

    StudentHashTable(StudentHashTable &&) = delete;

    StudentHashTable &operator=(StudentHashTable &&) = delete;

    void addStudent(Student student);

    void removeStudent(const Student &student) const;

    [[nodiscard]] MyString toString() const;

    void getStudentInfos(int &min, int &max, int &studentCount, int &teamCount) const;

};


inline std::ostream &operator<<(std::ostream &os, const StudentHashTable &table) {
    return os << table.toString();
}
