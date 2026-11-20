#pragma once

#include <iostream>

#include "MyString.h"

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
        studentGroup("-"),
        studentName(studentName) {
    }

    ~Student() = default;

    Student(Student &&otherStudent) noexcept;

    Student(const Student &otherStudent);

    Student &operator=(const Student &otherStudent);

    Student &operator=(Student &&otherStudent) noexcept;

    bool operator ==(const Student &otherStudent) const;

    bool operator !=(const Student &otherStudent) const;

    bool operator <(const Student &otherStudent) const;

    bool operator >(const Student &otherStudent) const;

    bool operator <=(const Student &otherStudent) const;

    bool operator >=(const Student &otherStudent) const;

    int getStudentId() const;

    const MyString& getStudentGroup() const;

    const MyString& getStudentName() const;

    bool isGroupEmpty() const;

    MyString toString() const;
};

std::ostream &operator<<(std::ostream& out, const Student& student);



