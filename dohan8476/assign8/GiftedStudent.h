#pragma once
#include "AdStudent.h"

class GiftedStudent : public AdStudent {
    MyString middleSchoolName;
    MyString parentName;
    MyString parentPhoneNumber;

    public:
        GiftedStudent();
        ~GiftedStudent() override;

        void input() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;
};
