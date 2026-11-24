#pragma once
#include "AdStudent.h"

//학번이 S로 시작!!
class ShortTermEduStudent : public AdStudent {
    MyString offerDepartment;

    public:
        ShortTermEduStudent();
        ~ShortTermEduStudent() override;

        void input() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;
};


