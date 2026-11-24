#pragma once
#include "AdStudent.h"

class LifeLongStudent : public AdStudent{
    int fee;

    public:
        LifeLongStudent();
        ~LifeLongStudent();

        void input() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;

};
