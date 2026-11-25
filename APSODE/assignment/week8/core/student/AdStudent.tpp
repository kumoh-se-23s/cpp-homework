#pragma once
#include "AdStudent.h"

template<typename Derived>
MyString AdStudent<Derived>::uid() {
    return Derived::get_uid();
}

template<typename Derived>
MyString AdStudent<Derived>::name() {
    return this->m_name;
}

template<typename Derived>
MyString AdStudent<Derived>::course() {
    return this->m_course;
}

template<typename Derived>
MyString AdStudent<Derived>::phone_number() {
    return this->m_phone_number;
}

template<typename Derived>
bool AdStudent<Derived>::operator==(const AdStudent &other) const {
    return this->uid() == other.uid()
           || this->phone_number() == other.phone_number();
}

template<typename Derived>
bool AdStudent<Derived>::operator!=(const AdStudent &other) const {
    return !(*this == other);
}

template<typename Derived>
bool AdStudent<Derived>::operator>(const AdStudent &other) const {
    return this->uid() > other.uid();
}

template<typename Derived>
bool AdStudent<Derived>::operator>=(const AdStudent &other) const {
    return !(*this < other);
}

template<typename Derived>
bool AdStudent<Derived>::operator<(const AdStudent &other) const {
    return other > *this;
}

template<typename Derived>
bool AdStudent<Derived>::operator<=(const AdStudent &other) const {
    return !(other > *this);
}
