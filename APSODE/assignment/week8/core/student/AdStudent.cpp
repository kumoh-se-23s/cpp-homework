#include "AdStudent.h"

AdStudent::AdStudent(const MyString &name, const MyString &course, const MyString &phone_number)
    : m_name(name), m_course(course), m_phone_number(phone_number) {
}

AdStudent::~AdStudent() = default;

void AdStudent::release_uid() const {
}

const MyString& AdStudent::name() const {
    return this->m_name;
}

const MyString& AdStudent::course() const {
    return this->m_course;
}

const MyString& AdStudent::phone_number() const {
    return this->m_phone_number;
}

bool AdStudent::operator==(const AdStudent &other) const {
    return this->uid() == other.uid()
           || this->phone_number() == other.phone_number();
}

bool AdStudent::operator==(const MyString &key) const {
    return this->uid() == key
           || this->phone_number() == key;
}

bool AdStudent::operator!=(const AdStudent &other) const {
    return !(*this == other);
}

bool AdStudent::operator<(const AdStudent &other) const {
    if (this->name() != other.name()) {
        return this->name() < other.name();
    }

    return this->phone_number() < other.phone_number();
}

bool AdStudent::operator>(const AdStudent &other) const {
    return other < *this;
}

bool AdStudent::operator>=(const AdStudent &other) const {
    return !(*this < other);
}

bool AdStudent::operator<=(const AdStudent &other) const {
    return !(other < *this);
}
