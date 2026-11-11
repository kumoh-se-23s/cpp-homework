//
// Created by Merutilm on 11/12/26.
//
#include "ShortTermEdu.h"

ShortTermEdu::ShortTermEdu() {
    static int cnt = 0;
    static constexpr int MAX = 5;
    key = "E";
    const MyString n = MyString::toString(++cnt);
    const int size = n.length();
    for (int i = 0; i < MAX - size; ++i) {
        key.append("0");
    }
    key.append(n);
}

void ShortTermEdu::input() {
    std::cout << "Input Modified Info with Course Dept Tel" << std::endl;
    std::cin >> course;
    std::cin >> department;
    std::cin >> phone;
}

MyString ShortTermEdu::toString() const {
    MyString result;
    return result.append("[ShortTermEdu] ")
            .append(name)
            .append("(")
            .append(key)
            .append(") ")
            .append(course)
            .append(" ")
            .append(department)
            .append(" ")
            .append(phone);
}

MyString ShortTermEdu::toFileFormattedString() const {
    MyString result;
    return result.append("ShortTermEdu ")
            .append(key)
            .append(" ")
            .append(name)
            .append(" ")
            .append(course)
            .append(" ")
            .append(department)
            .append(" ")
            .append(phone);
}
