//
// Created by Merutilm on 11/11/26.
//

#pragma once
#include "AdStudent.h"

struct LifeLong : AdStudent {
    int courseFee;

    void input() override {
        std::cout << "Input Modified Info with Course Amount Tel" << std::endl;
        std::cin >> course;
        std::cin >> courseFee;
        std::cin >> phone;
    }

    [[nodiscard]] MyString toString() const override {
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

    [[nodiscard]] MyString toFileFormattedString() const override {
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
};
