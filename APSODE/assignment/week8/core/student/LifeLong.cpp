#include "LifeLong.h"

LifeLong::LifeLong(const MyString &uid, const MyString &name, std::istream &input)
    : Visitable(name), m_uid(uid), m_tuition(0) {
    this->read_details(input);
}

const MyString& LifeLong::uid() const {
    return this->m_uid;
}

int LifeLong::tuition() const {
    return this->m_tuition;
}

void LifeLong::read_details(std::istream &input) {
    input >> this->m_course >> this->m_tuition >> this->m_phone_number;
}

void LifeLong::write(std::ostream &output) const {
    output << "LifeLong " << this->m_uid << ' ' << this->m_name << ' ' << this->m_course << ' '
           << this->m_tuition << ' ' << this->m_phone_number;
}
