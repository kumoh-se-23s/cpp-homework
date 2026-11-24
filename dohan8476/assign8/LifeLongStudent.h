#pragma once
#include "AdStudent.h"

class LifeLongStudent : public AdStudent{
    MyString studentID;
    int fee = 0;

    public:
        LifeLongStudent();
        LifeLongStudent(const MyString& studentID, const MyString& studentName, const MyString& course,
                    int fee, const MyString& phoneNumber);
        ~LifeLongStudent() override;

        const MyString& getID() const override;
        void input() override;
        void modify() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;

};
