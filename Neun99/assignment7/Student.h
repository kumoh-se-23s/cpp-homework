#pragma once
#include "MyString.h"

class Student {
public:
    //생성자
    Student(int newId, MyString newGroup, MyString newName);

    //getter
    int getId() const;
    const MyString& getName() const;
    const MyString& getGroup() const;
    MyString toString() const;

    //연산자 오버로딩
    Student& operator=(const Student& newStudent);
private:
    int id;
    MyString name;
    MyString group;

    MyString idToString() const;
};