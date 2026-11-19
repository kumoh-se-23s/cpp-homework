#pragma once
#include "CircularDoublyLinkedList.h"
#include "SinglyLinkedList.h"
#include "Student.h"
#include "LookUpTable.h"
#include "NestedDoubleNode.h"

class StudentController {
    using Id = LookUpTable<int, SinglyLinkedList<NestedDoubleNode<Student>>>;

    using Group = LookUpTable<MyString, CircularDoublyLinkedList<Student>>;

    Id studentIdList;

    Group studentGroupList;

    constexpr static int DEFAULT_CAPACITY = 8;

public:
    StudentController();
    StudentController();


    void insertStudent(int studentId, const MyString &studentGroup, const MyString &studentName);


    void deleteStudent(int studentId);

    MyString toString() const;

private:
     static int getYear(int studentId);
};

std::ostream& operator<<(std::ostream& out, const StudentController& studentController);

