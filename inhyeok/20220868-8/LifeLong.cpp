//
// Created by Merutilm on 11/12/26.
//
#include "LifeLong.h"

void LifeLong::input() {
    std::cout << "Input Modified Info with Course Amount Tel" << std::endl;
    std::cin >> course;
    std::cin >> courseFee;
    std::cin >> phone;
}

MyString LifeLong::toString() const {
    MyString result;
    return result.append("[LifeLong] ")
            .append(name)
            .append("(")
            .append(key)
            .append(") ")
            .append(course)
            .append(" ")
            .append(MyString::toString(courseFee))
            .append(" ")
            .append(phone);
}

MyString LifeLong::toFileFormattedString() const {
    MyString result;
    return result.append("LifeLong ")
            .append(key)
            .append(" ")
            .append(name)
            .append(" ")
            .append(course)
            .append(" ")
            .append(MyString::toString(courseFee))
            .append(" ")
            .append(phone);
}
