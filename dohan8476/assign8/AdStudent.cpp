#include "AdStudent.h"

AdStudent::~AdStudent() = default;

bool AdStudent::operator<(const AdStudent &other) const {
    if (this->studentName != other.studentName) {
        return this->studentName < other.studentName;
    }
    return this->phoneNumber < other.phoneNumber;
}

bool AdStudent::operator<=(const AdStudent &other) const {
    if (this->studentName != other.studentName) {
        return this->studentName <= other.studentName;
    }
    return this->phoneNumber <= other.phoneNumber;
}

bool AdStudent::operator==(const AdStudent &other) const {
    return this->studentName == other.studentName;
}

// 학번이나 폰번호로 조회
bool AdStudent::operator==(const MyString& key) const {
    return this->getID() == key || this->phoneNumber == key;
}

const MyString &AdStudent::getName() const {
    return this->studentName;
}

const MyString &AdStudent::getCourse() const {
    return this->course;
}

const MyString &AdStudent::getPhoneNumber() const {
    return this->phoneNumber;
}
