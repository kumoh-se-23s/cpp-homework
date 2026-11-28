#include "ShortTermEdu.h"

ShortTermEdu::ShortTermEdu(const MyString &uid,
                           const MyString &name, const MyString &course, const MyString &phone_number,
                           const MyString &host_depart)
    : Visitable(name, course, phone_number), m_uid(uid), m_host_depart(host_depart) {
    ShortTermEdu::observe(this->m_uid);
}

MyString ShortTermEdu::issue_uid() {
    return ShortTermEdu::issue();
}

const MyString& ShortTermEdu::uid() const {
    return this->m_uid;
}

const MyString& ShortTermEdu::host_depart() const {
    return this->m_host_depart;
}

void ShortTermEdu::release_uid() const {
    ShortTermEdu::release_if_last(this->m_uid);
}
