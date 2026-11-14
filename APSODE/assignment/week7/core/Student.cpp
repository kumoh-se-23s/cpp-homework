//
// Created by apsode on 26. 11. 14..
//

#include "Student.h"

Student::Student()
    : m_name(DEFAULT_NAME), m_uid(DEFAULT_UID), m_gid(DEFAULT_GID) {
}

Student::Student(const MyString &name, const MyString &uid, const MyString &gid)
    : m_name(name), m_uid(uid), m_gid(gid) {
}

Student::Student(const char *name, const char *uid, const char *gid)
    : m_name(name), m_uid(uid), m_gid(gid) {
}

MyString Student::name() const {
    return this->m_name;
}

MyString Student::uid() const {
    return this->m_uid;
}

MyString Student::gid() const {
    return this->m_gid;
}

MyString Student::set_name(const MyString &new_name) {
    this->m_name = new_name;
    return this->m_name;
}

MyString Student::set_uid(const MyString &new_uid) {
    this->m_uid = new_uid;
    return this->m_uid;
}

MyString Student::set_gid(const MyString &new_gid) {
    this->m_gid = new_gid;
    return this->m_gid;
}
