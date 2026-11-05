#pragma once

#include "MyString.h"

class Student {
    int studentID = 0;
    MyString studentName;
    MyString teamID;

    public:
        int getStudentID() const;
        MyString getStudentName();
        MyString getTeamID();

        void setStudentID(int studentID);
        void setStudentName(MyString studentName);
        void setTeamID(MyString teamID);
};
