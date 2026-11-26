#include "AdStudent.h"
#include <iostream>


AdStudent & AdStudent::operator=(AdStudent &&other) noexcept {
    if (this == &other)
        return *this;
    studentPhoneNumber = other.studentPhoneNumber;
    std::swap(studentName, other.studentName);
    return *this;
}

const MyString& AdStudent::getStudentPhoneNumber() const {
    return studentPhoneNumber;
}

const MyString& AdStudent::getStudentName() const {
    return studentName;
}

const MyString& AdStudent::getCourse() const {
    return course;
}

void AdStudent::setStudentPhoneNumber(const MyString &phoneNumber) {
    this->studentPhoneNumber = phoneNumber;
}

void AdStudent::setStudentName(const MyString &studentName) {
    this->studentName = studentName;
}

void AdStudent::setCourse(const MyString &course) {
    this->course = course;
}

int AdStudent::compareTo(const AdStudent &other) const {
    int studentNameCompare;
    int studentPhoneNumberCompare;
    if (this->studentName == other.studentName) {
        studentNameCompare = 0;
    } else if (this->studentName > other.studentName) {
        studentNameCompare = 1;
    } else {
        studentNameCompare = -1;
    }

    if (this->studentPhoneNumber == other.studentPhoneNumber) {
        studentPhoneNumberCompare = 0;
    } else if (this->studentPhoneNumber > other.studentPhoneNumber) {
        studentPhoneNumberCompare = 1;
    } else {
        studentPhoneNumberCompare = -1;
    }

    if (studentNameCompare == 0) {
        return studentPhoneNumberCompare;
    } else {
        return studentNameCompare;
    }
}

bool AdStudent::operator<(const AdStudent& other) const {
    return compareTo(other) < 0;
}

bool AdStudent::operator==(const AdStudent &other) const {
    return compareTo(other) == 0;
}

bool AdStudent::operator!=(const AdStudent &other) const {
    return compareTo(other) != 0;
}

bool AdStudent::operator>(const AdStudent &other) const {
    return compareTo(other) > 0;
}

bool AdStudent::operator<=(const AdStudent &other) const {
    return compareTo(other) <= 0;
}

bool AdStudent::operator>=(const AdStudent &other) const {
    return compareTo(other) >= 0;
}

bool AdStudent::operator==(const MyString& studentKey) const {
    return this->studentPhoneNumber !=  studentKey;
}

bool AdStudent::operator!=(const MyString& studentKey) const {
    return this->studentPhoneNumber != studentKey;
}

std::ostream &operator<<(std::ostream &out, const AdStudent &student) {
    out << student.toString();
    return out;
}