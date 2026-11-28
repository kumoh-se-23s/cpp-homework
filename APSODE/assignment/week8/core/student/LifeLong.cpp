#include "LifeLong.h"

LifeLong::LifeLong(const MyString &uid,
                   const MyString &name, const MyString &course, const MyString &phone_number,
                   int tuition)
    : Visitable(name, course, phone_number), m_uid(uid), m_tuition(tuition) {
}

const MyString& LifeLong::uid() const {
    return this->m_uid;
}

int LifeLong::tuition() const {
    return this->m_tuition;
}
