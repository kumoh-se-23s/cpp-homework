#include "Gifted.h"

Gifted::Gifted(const MyString &uid, const MyString &name, std::istream &input)
    : Visitable(name), m_uid(uid) {
    this->read_details(input);
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

void Gifted::read_details(std::istream &input) {
    input >> this->m_middle_school >> this->m_course >> this->m_phone_number
          >> this->m_parent_name >> this->m_parent_phone_number;
}

void Gifted::write(std::ostream &output) const {
    output << "Gifted " << this->m_uid << ' ' << this->m_name << ' ' << this->m_middle_school << ' '
           << this->m_course << ' ' << this->m_phone_number << ' '
           << this->m_parent_name << ' ' << this->m_parent_phone_number;
}
