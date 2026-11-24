#pragma once
#include <iostream>

#include "CircularDoublyLinkedList.h"
#include "OldSinglyLinkedList.h"
#include "Student.h"
#include "LookUpTable.h"
#include "NestedDoubleNode.h"

class OldStudentController {
    using IdNode = OldSingleNode<NestedDoubleNode<Student>>;
    using IdLinkedList = OldSinglyLinkedList<NestedDoubleNode<Student>>;
    using Id = LookUpTable<int, IdLinkedList>;

    using Group = LookUpTable<OOldMyString, CircularDoublyLinkedList<Student>>;

    Id studentIdList;

    Group studentGroupList;

    int studentCount = 0;

public:
    OldStudentController();

    OldStudentController(int minYear, int maxYear, int studentCount, int groupCount);

    ~OldStudentController();

    OldStudentController(const OldStudentController &) = delete;

    OldStudentController(OldStudentController &&other) noexcept;

    OldStudentController &operator=(const OldStudentController &) = delete;

    OldStudentController &operator=(OldStudentController &&other) noexcept;

    void insertStudent(int studentId, const OOldMyString &studentGroup, const OOldMyString &studentName);


    void deleteStudent(int studentId);

    OOldMyString toString() const;

    void print(std::ostream& out) const;

private:
    static int getYear(int studentId);

    void clearNoGroupNode();
};

std::ostream& operator<<(std::ostream& out, const OldStudentController& studentController);

std::ostream &operator<<(std::ostream &out, const NestedDoubleNode<Student> &nested);