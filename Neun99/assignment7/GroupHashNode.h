#pragma once
#include "MyString.h"
#include "StudentNode.h"

//그룹명을 key로, 그룹의 첫번째 학생의 주소를 value로 가지는 해시노드
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
