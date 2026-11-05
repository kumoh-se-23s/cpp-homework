#pragma once
#include "StudentNode.h"
#include "MyString.h"

class StudentCircularList {
public:
    //생성자
    StudentCircularList();

    //기능
    void addAscendly(int id, MyString name, MyString group);
    void addAfter(int id, MyString name, MyString group, StudentNode* preNode);
    MyString getAllStudentAscendly();
    MyString getAllStudentFromTarget(int id);
    void removeStudent(int id);

private:
    StudentNode* head;
    int size;
};