#pragma once
#include "AdStudent.h"

//학번이 S로 시작!!
class ShortTermEduStudent : public AdStudent {
    static int nextID;
    MyString studentID;
    MyString offerDepartment;

    static MyString generateID();

    public:
        ShortTermEduStudent();
        ShortTermEduStudent(const MyString& studentID, const MyString& studentName, const MyString& course,
                        const MyString& offerDepartment, const MyString& phoneNumber);
        ~ShortTermEduStudent();

        const MyString& getID() const override;
        void input() override;
        void modify() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;
};


