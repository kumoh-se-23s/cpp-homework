#include "AdStudent.h"
#include <iostream>

AdStudent & AdStudent::operator=(const AdStudent &other) {
    if (this == &other)
        return *this;
    studentPhoneNumber = other.studentPhoneNumber;
    studentName = other.studentName;
    return *this;
}

AdStudent & AdStudent::operator=(AdStudent &&other) noexcept {
    if (this == &other)
        return *this;
    studentPhoneNumber = other.studentPhoneNumber;
    std::swap(studentName, other.studentName);
    return *this;
}

const MyString &AdStudent::getStudentPhoneNumber() const {
    return studentPhoneNumber;
}

const MyString &AdStudent::getStudentName() const {
    return studentName;
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

bool AdStudent::operator==(const MyString& otherCourse) const {
    return this->course == otherCourse;
}

bool AdStudent::operator!=(const MyString& otherCourse) const {
    return this->course != otherCourse;
}

std::ostream &operator<<(std::ostream &out, const AdStudent &student) {
    student.print(out);
    return out;
}

std::istream &operator>>(std::istream &in, AdStudent &student) {
    student.input(in);
    return in;
}

std::ofstream &operator<<(std::ofstream &out, const AdStudent &student) {
    student.toFile(out);
    return out;
}
