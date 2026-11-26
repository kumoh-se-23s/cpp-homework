//
// Created by lhetl on 26. 11. 26..
//

#include "Gifted.h"
Gifted::Gifted(const MyArrayList<MyString> &input) : AdStudent(input.at(1), input.at(3), input.at(4)){
    this->studentId = input.at(0);
    this->middleSchoolName = input.at(2);
    this->parentName = input.at(5);
    this->parentPhoneNumber = input.at(6);
}

const MyString& Gifted::getStudentId() const {
    return studentId;
}

const MyString& Gifted::getMiddleSchoolName() const {
    return middleSchoolName;
}

const MyString& Gifted::getParentName() const {
    return parentName;
}

const MyString& Gifted::getParentPhoneNumber() const {
    return parentPhoneNumber;
}

bool Gifted::operator==(const MyString &studentKey) const {
    return (AdStudent::operator==(studentKey) || this->studentId == studentKey);
}

bool Gifted::operator!=(const MyString &studentKey) const {
    return (AdStudent::operator!=(studentKey) || this->studentId != studentKey);
}

MyString Gifted::toFileString() const {
}

MyString Gifted::toString() const {
}

void Gifted::update(const MyArrayList<MyString> &input) {
}
