#include "LifeLong.h"


LifeLong::LifeLong(const MyArrayList<MyString> &input) : AdStudent(input.at(1), input.at(2), input.at(4)) {
    this->studentId = input.at(0);
    this->tutionFee = stoi(input.at(3),0, 10);
}

const MyString& LifeLong::getStudentId() const {
    return studentId;
}

int LifeLong::getTutionFee() const {
    return tutionFee;
}

bool LifeLong::operator==(const MyString &studentKey) const {
    return (AdStudent::operator==(studentKey) || this->studentId == studentKey);
}

bool LifeLong::operator!=(const MyString &studentKey) const {
    return (AdStudent::operator!=(studentKey) || this->studentId != studentKey);
}

MyString LifeLong::toFileString() const {
}

MyString LifeLong::toString() const {
}

void LifeLong::update(const MyArrayList<MyString> &input) {
}
