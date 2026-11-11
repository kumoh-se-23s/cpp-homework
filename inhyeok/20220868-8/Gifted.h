//
// Created by Merutilm on 11/11/26.
//

#pragma once
#include "AdStudent.h"

struct Gifted : AdStudent {
    MyString middleSchoolName;
    MyString parentName;
    MyString parentPhone;

    void input() override {
        std::cout << "Input Modified Info with School Course Tel ParentName ParentTel" << std::endl;
        std::cin >> middleSchoolName;
        std::cin >> course;
        std::cin >> phone;
        std::cin >> parentName;
        std::cin >> parentPhone;
    }
    
    [[nodiscard]] MyString toString() const override {
        MyString result("[Gifted] ");
        return result.append(name)
                .append("(")
                .append(key)
                .append(") ")
                .append(middleSchoolName)
                .append(" ")
                .append(course)
                .append(" ")
                .append(phone)
                .append(" ")
                .append(parentName)
                .append("(")
                .append(parentPhone)
                .append(")");
    }
    [[nodiscard]] MyString toFileFormattedString() const override {
        MyString result("Gifted ");
        return result
                .append(key)
                .append(" ")
                .append(name)
                .append(" ")
                .append(middleSchoolName)
                .append(" ")
                .append(course)
                .append(" ")
                .append(phone)
                .append(" ")
                .append(parentName)
                .append(" ")
                .append(parentPhone);
    }
};
