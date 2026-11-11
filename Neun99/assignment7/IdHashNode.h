#pragma once
#include "StudentNode.h"

//학번의 년도와 해당 기수의 list head를 쌍으로 가지는 해시노드
class IdHashNode {
public:
    //생성자
    IdHashNode();
    IdHashNode(int id);

    //getter
    int getId() const;
    StudentNode* getHead() const;

    //setter
    void setId(int newId);
    void setHead(StudentNode* newHead);

    //기능
    bool isEmtpy() const;
private:
    int id; //학번 년도
    StudentNode* head; //list head
};