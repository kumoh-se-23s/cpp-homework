//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "MyString.h"

class Student {
    int num = 0;
    MyString name;
    int teamId;
public:

    constexpr static int NO_GROUP = -1;

    explicit Student(int num, MyString string, int teamId);

    [[nodiscard]] bool operator==(const Student& student) const;

    [[nodiscard]] int getYear() const;

    [[nodiscard]] int getTeamId() const;

    [[nodiscard]] const MyString &getName() const;

    [[nodiscard]] int makeYearHash() const;
    [[nodiscard]] int makeTeamHash() const;

    [[nodiscard]] MyString toString() const;
};