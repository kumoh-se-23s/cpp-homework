#pragma once
#include "AdStudent.h"

//학번이 E로 시작!!
class EmployEduStudent : public AdStudent{
    MyString companyName;
    MyString position;

    public:
        EmployEduStudent();
        ~EmployEduStudent() override;

        void input() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;
};
