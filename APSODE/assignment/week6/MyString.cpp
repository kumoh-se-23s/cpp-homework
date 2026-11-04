//
// Created by apsode on 26. 11. 2..
//

#include "MyString.h"

int MyString::length() const {
    return this->m_size;
}

bool MyString::isEmpty() const {
    return this->m_size == 0;
}

int MyString::getCapacity() const {
    return this->m_capacity;
}

int MyString::find(const MyString &subStr, int pos) const {
    return this->CharSequence::find(subStr, pos);
}

MyString MyString::substr(int pos, int len) const {
    
}

MyString& MyString::append(const MyString &str2nd) {
    CharSequence::append(str2nd);
    return *this;
}

MyString& MyString::insert(int pos, const MyString &subStr) {
    CharSequence::insert(pos, subStr);
    return *this;
}

MyString& MyString::replace(int pos, int len, const MyString &subStr) {
    MyString temp;
    return temp;
}

MyString& MyString::erase(int pos, int cnt) {
    Sequence::erase(pos, cnt);
    return *this;
}

char& MyString::operator[](int pos) {
    return CharSequence::operator[](pos);
}

const char& MyString::operator[](int pos) const {
    return CharSequence::operator[](pos);
}

MyString& MyString::operator+=(const MyString &str2nd) {
    return this->append(str2nd);
}

bool MyString::operator>(const MyString &str) const {
    return CharSequence::operator>(str.container);
}

bool MyString::operator<(const MyString &str) const {
    return CharSequence::operator<(str.container);
}

bool MyString::operator>=(const MyString &str) const {
    return CharSequence::operator>=(str.container);
}

bool MyString::operator<=(const MyString &str) const {
    return CharSequence::operator<=(str.container);
}

int stoi(const MyString &str, int pos, int base) {
}
