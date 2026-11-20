#pragma once
#include "MyString.h"
#include <iostream>

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
    // Student(const MyString& studentId, const MyString &studentGroup, const MyString &studentName)
    //     : studentId(stoi(studentId, 10)),
    //     studentGroup(studentGroup),
    //     studentName(studentName) {
    // }
    // Student(const MyString& studentId, const MyString &studentName)
    //     : studentId(stoi(studentId, 10)),
    //     studentGroup("-"),
    //     studentName(studentName) {
    // }


    Student(int studentId, const MyString &studentName)
        : studentId(studentId),
        studentName(studentName),
        studentGroup("-") {
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



