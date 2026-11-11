//
// Created by Merutilm on 11/11/26.
//

#pragma once
#include "AdStudent.h"

struct ShortTermEdu : AdStudent {
    MyString department;

    explicit ShortTermEdu() {
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

    void input() override {
        std::cout << "Input Modified Info with Course Dept Tel" << std::endl;
        std::cin >> course;
        std::cin >> department;
        std::cin >> phone;
    }

    [[nodiscard]] MyString toString() const override {
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

    [[nodiscard]] MyString toFileFormattedString() const override {
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
};
