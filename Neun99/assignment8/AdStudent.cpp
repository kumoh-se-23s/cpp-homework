#include "AdStudent.h"

AdStudent::AdStudent(const MyString &name, const MyString &course, const MyString &tel) {
    this->name = name;
    this->course = course;
    this->tel = tel;
}

AdStudent::~AdStudent() {}

//getter------------------------------------------
const MyString &AdStudent::getName() const {
    return name;
}

const MyString &AdStudent::getCourse() const {
    return course;
}

const MyString &AdStudent::getTel() const {
    return tel;
}

// setter---------------------------------------------
void AdStudent::setName(const MyString &name) {
    this->name = name;
}

void AdStudent::setCourse(const MyString &course) {
    this->course = course;
}

void AdStudent::setTel(const MyString &tel) {
    this->tel = tel;
}

// 연산자오버로딩------------------------------------------
bool AdStudent::operator==(const AdStudent &std2) const {
    if (name == std2.name)
        return tel == std2.tel;
    return false;
}

bool AdStudent::operator!=(const AdStudent &std2) const {
    return this != &std2;
}

bool AdStudent::operator>(const AdStudent &std2) const {
    if (name == std2.name)
        return tel > std2.tel;
    return name > std2.name;
}

bool AdStudent::operator>=(const AdStudent &std2) const {
    if (name == std2.name)
        return tel >= std2.tel;
    return name > std2.name;
}

bool AdStudent::operator<(const AdStudent &std2) const {
    if (name == std2.name)
        return tel < std2.tel;
    return name < std2.name;
}

bool AdStudent::operator<=(const AdStudent &std2) const {
    if (name == std2.name)
        return tel <= std2.tel;
    return name < std2.name;
}