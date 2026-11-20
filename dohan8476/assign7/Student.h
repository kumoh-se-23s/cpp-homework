#pragma once

#include "MyString.h"

class Student {
    //생각해보니까 이거 MyString하고 앞 4자리만 가져오는게 맞는거 같은데 너무 멀리 와버렷다
    int studentID;
    MyString studentName;
    MyString teamID;

    public:
        Student();
        Student(int studentID, const MyString& studentName, const MyString& teamID);

        int getStudentID() const;
        const MyString& getStudentName() const;
        const MyString& getTeamID() const;

        void setStudentID(int studentID);
        void setStudentName(const MyString& studentName);
        void setTeamID(const MyString& teamID);

        int getYear() const; // 학번 앞 4자리
        bool hasTeam() const; // 팀 여부

        bool operator<(const Student& other) const;
        bool operator==(const Student& other) const;
        bool operator==(int otherID) const;
};