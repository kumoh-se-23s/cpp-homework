#pragma once
#include "AdStudent.h"

class GiftedStudent : public AdStudent {
    MyString studentID;
    MyString middleSchoolName;
    MyString parentName;
    MyString parentPhoneNumber;

    public:
        GiftedStudent();
        //id, 이름, 중학교, 과정, 전번, 부모성함, 부모전번
        GiftedStudent(const MyString& studentID, const MyString& studentName,
            const MyString& middleSchoolName, const MyString& course, const MyString& phoneNumber,
                  const MyString& parentName, const MyString& parentPhoneNumber);
        ~GiftedStudent() override;

        const MyString& getID() const override { return studentID; }
        void input() override;
        void modify() override;
        MyString toString() const override;
        MyString toFileFormatString() const override;
};
