#pragma once
#include "MyString.h"
#include "StudentNode.h"
#include "IdHashMap.h"
#include "GroupHashMap.h"

class StudentMultiList {
public:
    StudentMultiList();
    ~StudentMultiList();

    void printAll();
    void insertStudent(int id, MyString group, MyString name);
    void deleteStudent(int id);
private:
    IdHashMap idMap;
    GroupHashMap groupMap;

    //원형 더블 연결 처리
    void teammateInsert(StudentNode* newNode);
    void teammateDelete();

    //단일 연결 처리
    void classmateInsert(StudentNode* newNode);
    void classmateInsertFront(StudentNode* newNode);
    void classmateInsertAfter(StudentNode* newNode, StudentNode* preNode);
    void classmateInsertLast(StudentNode* newNode, StudentNode* preNode);

    void classmateDelete(MyString group);
    void classmateDeleteFront();
    void classmateDeleteAfter(StudentNode* preNode);
    void classmateDeleteLast(StudentNode* preNode);
};