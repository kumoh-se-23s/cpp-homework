#include "Gifted.h"

Gifted::Gifted(const MyString &uid,
               const MyString &name, const MyString &course, const MyString &phone_number,
               const MyString &middle_school, const MyString &parent_name, const MyString &parent_phone_number)
    : Visitable(name, course, phone_number), m_uid(uid), m_middle_school(middle_school),
      m_parent_name(parent_name), m_parent_phone_number(parent_phone_number) {
}

const MyString& Gifted::uid() const {
    return this->m_uid;
}

const MyString& Gifted::middle_school() const {
    return this->m_middle_school;
}

const MyString& Gifted::parent_name() const {
    return this->m_parent_name;
}

const MyString& Gifted::parent_phone_number() const {
    return this->m_parent_phone_number;
}
