#pragma once
#include "AdStudent.h"

//학번이 E로 시작!!
class EmployEduStudent : public AdStudent{
    static int nextID;
    MyString studentID;
    MyString companyName;
    MyString position;

    static MyString generateID();

    public:
        EmployEduStudent();
        EmployEduStudent(const MyString& studentID, const MyString& studentName, const MyString& companyName,
                     const MyString& position, const MyString& course, const MyString& phoneNumber);
        ~EmployEduStudent() override;

        const MyString& getID() const override;
        void input() override;
        void modify() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;
};
