#pragma once
#include <iostream>

#include "CircularDoublyLinkedList.h"
#include "SinglyLinkedList.h"
#include "Student.h"
#include "LookUpTable.h"
#include "NestedDoubleNode.h"

class StudentController {
    using IdNode = SingleNode<NestedDoubleNode<Student>>;
    using IdLinkedList = SinglyLinkedList<NestedDoubleNode<Student>>;
    using Id = LookUpTable<int, IdLinkedList>;

    using Group = LookUpTable<MyString, CircularDoublyLinkedList<Student>>;

    Id studentIdList;

    Group studentGroupList;

    int studentCount = 0;

public:
    StudentController();

    StudentController(int minYear, int maxYear, int studentCount, int groupCount);


    void insertStudent(int studentId, const MyString &studentGroup, const MyString &studentName);


    void deleteStudent(int studentId);

    MyString toString() const;

    void print(std::ostream& out) const;

private:
    static int getYear(int studentId);
};

std::ostream& operator<<(std::ostream& out, const StudentController& studentController);

std::ostream &operator<<(std::ostream &out, const NestedDoubleNode<Student> &nested);