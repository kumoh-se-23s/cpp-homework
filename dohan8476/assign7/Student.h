#pragma once

#include "MyString.h"

class Student {
    int studentID;
    MyString studentName;
    MyString teamID;

    public:
        Student();
        Student(int studentID, MyString studentName, MyString teamID);

        int getStudentID() const;
        MyString getStudentName() const;
        MyString getTeamID() const;

        void setStudentID(int studentID);
        void setStudentName(MyString studentName);
        void setTeamID(MyString teamID);

        int getYear() const; // 학번 앞 4자리
        bool hasTeam() const; // 팀 여부

        bool operator<(const Student& other) const;
        bool operator==(const Student& other) const;
};
