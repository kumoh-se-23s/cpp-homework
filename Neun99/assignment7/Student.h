#pragma once
#include "MyString.h"

class Student {
public:
    //생성자
    Student(int newId, MyString newGroup, MyString newName);

    //getter
    int getId() const;
    MyString getName() const;
    MyString getGroup() const;

    //setter
    void setId(int newId);
    void setName(MyString newName);
    void setGroup(MyString newGroup);

    //연산자 오버로딩
    Student& operator=(const Student& newStudent);
private:
    int id;
    MyString name;
    MyString group;
};