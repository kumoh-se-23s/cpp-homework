//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "MyString.h"

class Student {
    int num = 0;
    MyString name;
    MyString groupId;
public:

    inline static const MyString NO_GROUP = "";

    explicit Student(int num, MyString string = "", MyString groupId = NO_GROUP);

    [[nodiscard]] bool operator==(const Student& student) const;

    [[nodiscard]] int getNum() const;

    [[nodiscard]] int getYear() const;

    [[nodiscard]] MyString &getGroupId();

    [[nodiscard]] const MyString &getGroupId() const;

    [[nodiscard]] const MyString &getName() const;

    [[nodiscard]] MyString toFormatString() const;

    [[nodiscard]] MyString toString() const;
};