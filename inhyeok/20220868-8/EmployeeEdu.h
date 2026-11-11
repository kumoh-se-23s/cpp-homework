//
// Created by Merutilm on 11/11/26.
//

#pragma once
#include <iomanip>

#include "AdStudent.h"
#include "MyString.h"

struct EmployeeEdu : AdStudent {
    MyString companyName;
    MyString position;


    explicit EmployeeEdu() {
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
        std::cout << "Input Modified Info with Company Position Course Tel" << std::endl;
        std::cin >> companyName;
        std::cin >> position;
        std::cin >> course;
        std::cin >> phone;
    }

    [[nodiscard]] MyString toString() const override {
        MyString result;
        return result.append("[EmployeeEdu] ")
                .append(name)
                .append("(")
                .append(key)
                .append(") ")
                .append(course)
                .append(" ")
                .append(position)
                .append(" ")
                .append(companyName)
                .append(" ")
                .append(phone);
    }

    [[nodiscard]] MyString toFileFormattedString() const override {
        MyString result;
        return result.append("EmployeeEdu ")
                .append(key)
                .append(" ")
                .append(name)
                .append(" ")
                .append(course)
                .append(" ")
                .append(position)
                .append(" ")
                .append(companyName)
                .append(" ")
                .append(phone);
    }
};
