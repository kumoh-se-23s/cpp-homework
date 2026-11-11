#pragma once
#include "MyString.h"
#include "StudentNode.h"

class GroupHashNode {
public:
    //생성자
    GroupHashNode();
    GroupHashNode(MyString group);

    //getter
    MyString getId() const;
    StudentNode* getHead() const;

    //setter
    void setId(MyString newId);
    void setHead(StudentNode* newHead);

    //기능
    bool isEmpty() const;
private:
    MyString id;
    StudentNode* head;
};
