//
// Created by apsode on 26. 11. 2..
//

#if 0 // TODO: MyString 작성 중이라 임시로 빌드에서 제외. 완성하면 이 줄과 파일 끝의 #endif를 지울 것
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

char& MyString::operator[](int pos) {
    return CharSequence::operator[](pos);
}

const char& MyString::operator[](int pos) const {
    return CharSequence::operator[](pos);
}

MyString& MyString::operator=(const char *cstr2nd) {
    MyString temp_string = MyString(cstr2nd);
    swap(*this, temp_string);
    return *this;
}

bool MyString::operator==(const char *cstr2nd) const {
    return this->equals(MyString(cstr2nd));
}

bool MyString::operator!=(const char *cstr2nd) const {
    return !this->equals(MyString(cstr2nd));
}

int MyString::find(const char *cSubStr, int pos) const {
    return CharSequence::find(cSubStr, pos);
}

int MyString::find(const MyString &subStr, int pos) const {
    return CharSequence::find(subStr, pos);
}

MyString& MyString::operator+=(const char *cstr2nd) {
    return this->append(cstr2nd);
}

MyString& MyString::operator+=(const MyString &str2nd) {
    return this->append(str2nd);
}

MyString& MyString::append(const char *cstr2nd) {
    Sequence::append(cstr2nd, count_elements(cstr2nd));
    return *this;
}

MyString& MyString::append(const MyString &str2nd) {
    Sequence::append(str2nd.container, str2nd.m_size);
    return *this;
}

bool MyString::operator<(const MyString &str) const {
    return this->compare_to(str) < 0;
}

bool MyString::operator<=(const MyString &str) const {
    return this->compare_to(str) <= 0;
}

bool MyString::operator>(const MyString &str) const {
    return this->compare_to(str) > 0;
}

bool MyString::operator>=(const MyString &str) const {
    return this->compare_to(str) >= 0;
}

bool MyString::operator<(const char *cstr2nd) const {
    return this->compare_to(MyString(cstr2nd)) < 0;
}

bool MyString::operator<=(const char *cstr2nd) const {
    return this->compare_to(MyString(cstr2nd)) <= 0;
}

bool MyString::operator>(const char *cstr2nd) const {
    return this->compare_to(MyString(cstr2nd)) > 0;
}

bool MyString::operator>=(const char *cstr2nd) const {
    return this->compare_to(MyString(cstr2nd)) >= 0;
}

MyString& MyString::insert(int pos, const char *subStr) {
    Sequence::insert(pos, *subStr);
    return *this;
}

MyString& MyString::insert(int pos, const MyString &subStr) {
    Sequence::insert(pos, subStr.container, subStr.m_size);
    return *this;
}

MyString& MyString::replace(int pos, int len, const char *subStr) {
    require_non_null(subStr);
    require_valid_range(subStr, count_elements(subStr));
    Sequence::insert(pos, subStr, )
}

MyString& MyString::replace(int pos, int len, const MyString &subStr) {
}

MyString& MyString::erase(int pos, int cnt) {
    Sequence::erase(pos, cnt);
    return *this;
}

int MyString::compare_to(const MyString &other) const {
}

int stoi(const MyString &str, int pos, int base) {
}
#endif


