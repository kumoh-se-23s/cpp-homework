#pragma once

#include <iostream>

#include "OOldMyString.h"

class Student {
    int studentId;
    OOldMyString studentGroup;
    OOldMyString studentName;


public:
    Student(int studentId, const OOldMyString &studentGroup, const OOldMyString &studentName)
        : studentId(studentId),
        studentGroup(studentGroup),
        studentName(studentName) {
    }


    Student(int studentId, const OOldMyString &studentName)
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

    const OOldMyString& getStudentGroup() const;

    const OOldMyString& getStudentName() const;

    bool isGroupEmpty() const;

    OOldMyString toString() const;
};

std::ostream &operator<<(std::ostream& out, const Student& student);



