#include<iostream>
#include "ShortTermEdu.h"

ShortTermEdu::ShortTermEdu(const MyArrayList<MyString> &input) : AdStudent(input.at(0), input.at(1), input.at(3)) {
    this->studentId = "S";
    MyString stringValue = MyString::integerToString(++LAST_STUDENT_ID);

    for (int i = stringValue.length(); i <= MAX_STUDENT_ID_SIZE; ++i) {
        studentId.append("0");
    }
    this->studentId.append(stringValue);
    
    this->department = input.at(2);
}

ShortTermEdu::ShortTermEdu(const MyString &studentId, const MyArrayList<MyString> &input) : AdStudent(input.at(1), input.at(2), input.at(4)) {
    this->studentId = studentId;
    int studentIdValue = stoi(studentId, 1, 10);
    LAST_STUDENT_ID = std::max(studentIdValue, LAST_STUDENT_ID);

    this->department = input.at(2);
}

const MyString& ShortTermEdu::getStudentId() const {
    return studentId;
}

const MyString& ShortTermEdu::getDepartment() const {
    return department;
}

bool ShortTermEdu::operator==(const MyString &studentKey) const {
    return (AdStudent::operator==(studentKey) || this->studentId == studentKey);
}

bool ShortTermEdu::operator!=(const MyString &studentKey) const {
    return (AdStudent::operator!=(studentKey) || this->studentId != studentKey);
}

MyString ShortTermEdu::toFileString() const {
}

MyString ShortTermEdu::toString() const {
}

void ShortTermEdu::update(const MyArrayList<MyString> &input) {
}
