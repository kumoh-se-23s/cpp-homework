#pragma once
#include "MyString.h"
#include "StudentNode.h"

class GroupHashNode {
public:
    //생성자
    GroupHashNode();
    GroupHashNode(const MyString& group);

    //getter
    const MyString& getId() const;
    StudentNode* getHead() const;

    //setter
    void setId(const MyString& newId);
    void setHead(StudentNode* newHead);

    //기능
    bool isEmpty() const;
private:
    MyString id;
    StudentNode* head;
};
