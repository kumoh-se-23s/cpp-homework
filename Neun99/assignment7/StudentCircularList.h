#pragma once
#include "StudentNode.h"
#include "MyString.h"

//이 곳에선 노드의 생성 소멸을 관리하지 않음, 모두 외부에서 노드를 받아서 작업
class StudentCircularList {
public:
    //생성자
    StudentCircularList();
private:
    StudentNode* head;
    int size;
};