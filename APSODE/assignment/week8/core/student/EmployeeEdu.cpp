#include "EmployeeEdu.h"

EmployeeEdu::EmployeeEdu(const MyString &uid,
                         const MyString &name, const MyString &course, const MyString &phone_number,
                         const MyString &company, const MyString &position)
    : Visitable(name, course, phone_number), m_uid(uid), m_company(company), m_position(position) {
    EmployeeEdu::observe(this->m_uid);
}

MyString EmployeeEdu::issue_uid() {
    return EmployeeEdu::issue();
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

void EmployeeEdu::release_uid() const {
    EmployeeEdu::release_if_last(this->m_uid);
}
