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

MyString Student::toString() const {
    MyString result;
    result += MyString::integerToString(studentId);

    result += " ";

    result += studentGroup;

    result += " ";

    result += studentName;

    return result;
}

std::ostream &operator<<(std::ostream &out, const Student &student) {
    out << student.getStudentId() << " " << student.getStudentName();
    return out;
}
