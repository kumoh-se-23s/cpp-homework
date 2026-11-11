#pragma once
#include "../assignment6/MyString.h"

class Student {
    int studentId;
    MyString studentGroup;
    MyString studentName;


public:
    Student(int studentId, const MyString &studentGroup, const MyString &studentName)
        : studentId(studentId),
        studentGroup(studentGroup),
        studentName(studentName) {
    }

    Student(int studentId, const MyString &studentName)
        : studentId(studentId),
        studentName(studentName),
        studentGroup("-") {
    }

    bool operator ==(int otherStudentId) const;

    bool operator !=(int otherStudentId) const;

    bool operator <(int otherStudentId) const;

    bool operator >(int otherStudentId) const;

    int getStudentId() const;

    const MyString& getStudentGroup() const;

    const MyString& getStudentName() const;
};


