#include "ShortTermEdu.h"

ShortTermEdu::ShortTermEdu(const MyString &name, std::istream &input)
    : ShortTermEdu(ShortTermEdu::issue(), name, input) {
}

ShortTermEdu::ShortTermEdu(const MyString &uid, const MyString &name, std::istream &input)
    : Visitable(name), m_uid(uid) {
    ShortTermEdu::observe(this->m_uid);
    this->read_details(input);
}

const MyString& ShortTermEdu::uid() const {
    return this->m_uid;
}

const MyString& ShortTermEdu::host_depart() const {
    return this->m_host_depart;
}

void ShortTermEdu::read_details(std::istream &input) {
    input >> this->m_course >> this->m_host_depart >> this->m_phone_number;
}

void ShortTermEdu::write(std::ostream &output) const {
    output << "ShortTermEdu " << this->m_uid << ' ' << this->m_name << ' ' << this->m_course << ' '
           << this->m_host_depart << ' ' << this->m_phone_number;
}

void ShortTermEdu::release_uid() const {
    ShortTermEdu::release_if_last(this->m_uid);
}
