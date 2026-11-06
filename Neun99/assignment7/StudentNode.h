#pragma once
#include "Student.h"

class StudentNode {
public:
    //생성자
    StudentNode(int id, MyString group, MyString name);

    //getter
    const Student& getStudent() const;
    StudentNode* getNextTeammate() const;
    StudentNode* getPrevTeammate() const;
    StudentNode* getNextClassmate() const;

    //setter
    void setStudent(const Student& newStudent);
    void setNextTeammate(StudentNode* newNextTeammate);
    void setPrevTeammate(StudentNode* newPrevTeammate);
    void setNextClassmate(StudentNode* newNextClassmate);
private:
    Student student;
    StudentNode* nextTeammate = nullptr;
    StudentNode* prevTeammate = nullptr;
    StudentNode* nextClassmate = nullptr;
};