//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "Student.h"
#include <stop_token>

#include "RAIISharedStudentNode.h"
#include "StudentNode.h"

class StudentHashTable {
    RAIISharedStudentNode *studentTable = nullptr;
    int studentNumHashTableCapacity = 0;
    int studentTableWeight = 0;

    RAIISharedStudentNode *teamTable = nullptr;
    int teamNumHashTableCapacity = 0;
    int teamTableWeight = 0;

public:
    explicit StudentHashTable(int initialCapacity = 0);;

    ~StudentHashTable();

    StudentHashTable(const StudentHashTable &) = delete;

    StudentHashTable &operator=(const StudentHashTable &) = delete;

    StudentHashTable(StudentHashTable &&) = delete;

    StudentHashTable &operator=(StudentHashTable &&) = delete;

    void addStudent(Student student);

    void removeStudent(const Student &student);

    [[nodiscard]] MyString toString() const;

    void getStudentMinMaxNum(int &min, int &max) const;

private:
    void resize(int newStudentTableCapacity, int newTeamTableCapacity);

    static int hash(int v, int size);

    void addStudent(RAIISharedStudentNode *currStudentTable, int currStudentNumHashTableCapacity, RAIISharedStudentNode *currTeamTable, int currTeamNumHashTableCapacity, Student student);
};


inline std::ostream &operator<<(std::ostream &os, const StudentHashTable &table) {
    return os << table.toString();
}
