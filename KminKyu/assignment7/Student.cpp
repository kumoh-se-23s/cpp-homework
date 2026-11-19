#include "Student.h"
#include "MyString.h"

Student::Student(Student &&otherStudent) noexcept {
    this->studentGroup = std::move(otherStudent.studentGroup);
    this->studentName = std::move(otherStudent.studentName);
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
    this->studentGroup = std::move(otherStudent.studentGroup);
    this->studentName = std::move(otherStudent.studentName);
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

bool Student::isGroupEmpty() const {
    return studentGroup == "-";
}

MyString Student::toString() const {
    MyString result;
    result += MyString::integerToString(studentId);
    result += studentName;
    result += " (";
    if (!isGroupEmpty()) {
        result += "[";
        result += studentGroup;
        result += "]";
    }
    return result;
}

bool Student::operator==(const Student &otherStudent) const {
    return studentId == otherStudent.studentId;
}

bool Student::operator!=(const Student &otherStudent) const {
    return studentId != otherStudent.studentId;
}

bool Student::operator<(const Student &otherStudent) const {
    return studentId < otherStudent.studentId;
}

bool Student::operator>(const Student &otherStudent) const {
    return studentId > otherStudent.studentId;
}


bool Student::operator<=(const Student &otherStudent) const {
    return studentId <= otherStudent.studentId;
}
bool Student::operator>=(const Student &otherStudent) const {
    return studentId >= otherStudent.studentId;
}
