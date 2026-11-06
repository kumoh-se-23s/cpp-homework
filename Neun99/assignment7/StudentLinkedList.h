#pragma once
#include "StudentNode.h"
#include "MyString.h"

//이곳에서 Node 생성과 삭제를 전담관리
class StudentLinkedList {
public:
    //생성자
    StudentLinkedList();
    ~StudentLinkedList();

    //기능
    void addAscendly(int id, MyString group, MyString name);
    void removeStudent(int id);
private:
    StudentNode* head;
    int size;

    StudentNode* findById(int id);
    StudentNode* findPreNodeById(int id);

    void addFirst(StudentNode* newStudent);
    void addAfter(StudentNode* newStudent, StudentNode* preNode);
    void addLast(StudentNode* newStudent);

    void removeFirst();
    void removeAfter(StudentNode* preNode);
    void removeLast();

    void deleteAll(StudentNode* node);
};