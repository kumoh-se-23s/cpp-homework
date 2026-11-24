#pragma once
#include "MyString.h"

class AdStudent {
    MyString StudentID; // key
    MyString StudentName;
    MyString course;
    MyString phoneNumber;

    public:
        virtual ~AdStudent();

        bool operator <(const AdStudent& other) const;
        bool operator <=(const AdStudent& other) const;
        bool operator ==(const AdStudent& other) const;

        virtual void input();
        virtual MyString toString() const;
        virtual MyString toFileFormatString() const;
};