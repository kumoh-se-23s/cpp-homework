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


    explicit EmployeeEdu();

    void input() override;

    [[nodiscard]] MyString toString() const override;

    [[nodiscard]] MyString toFileFormattedString() const override;
};
