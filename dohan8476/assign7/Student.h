#pragma once

#include "MyString.h"

class Student {
    MyString studentID;
    MyString studentName;
    MyString teamID;

    public:
        MyString getStudentID();
        MyString getStudentName();
        MyString getTeamID();

        void setStudentID(MyString studentID);
        void setStudentName(MyString studentName);
        void setTeamID(MyString teamID);
};
