#pragma once
#include "MyString.h"

class AdStudent {
    protected:
        //[II-1-1] 학번은 재직자과정과 단기특강과정에서는 부여 방식이 다르므로,
        //반드시 파생클래스가 가지게 구현한다.
        MyString studentName;
        MyString course;
        MyString phoneNumber;

    public:
        virtual ~AdStudent();

        bool operator <(const AdStudent& other) const;
        bool operator <=(const AdStudent& other) const;
        bool operator ==(const AdStudent& other) const;
        bool operator ==(const MyString& key) const;

        [[nodiscard]] const MyString& getName() const;
        [[nodiscard]] const MyString& getCourse() const;
        [[nodiscard]] const MyString& getPhoneNumber() const;

        virtual const MyString& getID() const = 0;
        virtual void input() = 0;
        virtual void modify() = 0;
        virtual MyString toString() const = 0;
        virtual MyString toFileFormatString() const = 0;
};