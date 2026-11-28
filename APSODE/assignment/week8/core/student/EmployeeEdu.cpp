#include "EmployeeEdu.h"

EmployeeEdu::EmployeeEdu(const MyString &name, std::istream &input)
    : EmployeeEdu(EmployeeEdu::issue(), name, input) {
}

EmployeeEdu::EmployeeEdu(const MyString &uid, const MyString &name, std::istream &input)
    : Visitable(name), m_uid(uid) {
    EmployeeEdu::observe(this->m_uid);
    this->read_details(input);
}

const MyString& EmployeeEdu::uid() const {
    return this->m_uid;
}

const MyString& EmployeeEdu::company() const {
    return this->m_company;
}

const MyString& EmployeeEdu::position() const {
    return this->m_position;
}

void EmployeeEdu::read_details(std::istream &input) {
    input >> this->m_company >> this->m_position >> this->m_course >> this->m_phone_number;
}

void EmployeeEdu::write(std::ostream &output) const {
    output << "EmployeeEdu " << this->m_uid << ' ' << this->m_name << ' ' << this->m_company << ' '
           << this->m_position << ' ' << this->m_course << ' ' << this->m_phone_number;
}

void EmployeeEdu::release_uid() const {
    EmployeeEdu::release_if_last(this->m_uid);
}
