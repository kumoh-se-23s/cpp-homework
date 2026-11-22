//
// Created by lhetl on 2026-11-22.
//

#include "AdStudent.h"

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

int AdStudent::getStudentPhoneNumber() const {
    return studentPhoneNumber;
}

MyString AdStudent::getStudentName() const {
    return studentName;
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
