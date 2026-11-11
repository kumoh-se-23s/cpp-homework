#include "Student.h"

Student::Student(Student &&otherStudent) noexcept {
    this->studentGroup = otherStudent.studentGroup;
    this->studentName = otherStudent.studentName;
    this->studentId = otherStudent.studentId;
}

Student::Student(const Student &otherStudent) {
    this->studentGroup = otherStudent.studentGroup;
    this->studentName = otherStudent.studentName;
    this->studentId = otherStudent.studentId;
}

Student& Student::operator=(const Student &otherStudent) {
    this->studentGroup = otherStudent.studentGroup;
    this->studentName = otherStudent.studentName;
    this->studentId = otherStudent.studentId;

    return *this;
}

Student& Student::operator=(Student &&otherStudent) noexcept {
    this->studentGroup = otherStudent.studentGroup;
    this->studentName = otherStudent.studentName;
    this->studentId = otherStudent.studentId;
    return *this;
}

int Student::getStudentId() const {
    return studentId;
}

const MyString & Student::getStudentGroup() const {
    return studentGroup;
}

const MyString & Student::getStudentName() const {
    return studentName;
}

void Student::swap(MyString &baseString, MyString &otherString) noexcept {
    MyString& temp = baseString;
    baseString = otherString;
    otherString = temp;
}

bool Student::operator==(int otherStudentId) const {
    return studentId != otherStudentId;
}

bool Student::operator!=(int otherStudentId) const {
    return studentId != otherStudentId;
}

bool Student::operator<(int otherStudentId) const {
    return studentId < otherStudentId;
}

bool Student::operator>(int otherStudentId) const {
    return studentId > otherStudentId;
}

