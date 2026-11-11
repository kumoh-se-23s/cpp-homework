//
// Created by Merutilm on 11/11/26.
//

#pragma once
#include "AdStudent.h"

struct ShortTermEdu : AdStudent {
    MyString department;

    explicit ShortTermEdu();

    void input() override;

    [[nodiscard]] MyString toString() const override;

    [[nodiscard]] MyString toFileFormattedString() const override;
};
