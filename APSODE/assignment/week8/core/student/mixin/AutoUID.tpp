#pragma once
#include "AutoUID.h"

template<typename Derived, char Prefix>
int AutoUID<Derived, Prefix>::s_last = 0;

template<typename Derived, char Prefix>
MyString AutoUID<Derived, Prefix>::issue() {
    return format(++s_last);
}

template<typename Derived, char Prefix>
void AutoUID<Derived, Prefix>::observe(const MyString &uid) {
    int number = number_of(uid);
    if (number > s_last) {
        s_last = number;
    }
}

template<typename Derived, char Prefix>
void AutoUID<Derived, Prefix>::release_if_last(const MyString &uid) {
    if (number_of(uid) == s_last) {
        --s_last;
    }
}

template<typename Derived, char Prefix>
MyString AutoUID<Derived, Prefix>::format(int number) {
    MyString uid = MyString();
    uid.append(Prefix);

    for (int count = 0, rest = number; count < DIGITS; ++count, rest /= RADIX) {
        uid.insert(1, static_cast<char>('0' + rest % RADIX));
    }

    return uid;
}

template<typename Derived, char Prefix>
int AutoUID<Derived, Prefix>::number_of(const MyString &uid) {
    return stoi(uid, 1);
}
