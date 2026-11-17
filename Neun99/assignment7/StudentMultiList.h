#pragma once
#include "MyString.h"
#include "StudentNode.h"
#include "IdHashMap.h"
#include "GroupHashMap.h"

class StudentMultiList {
public:
    StudentMultiList();
    ~StudentMultiList();

    MyString toStringForSave() const;
    MyString toStringForPrint() const;
    void insertStudent(int id, const MyString& group, const MyString& name);
    bool removeStudent(int id);
private:
    int studentCnt;
    IdHashMap idMap;
    GroupHashMap groupMap;
    const MyString NO_GROUP = "-";

    //단일 연결 처리
    void classmateInsert(StudentNode* newNode);
    void classmateInsertFront(IdHashNode* idHash, StudentNode* newNode);
    void classmateInsertAfter(StudentNode* newNode, StudentNode* preNode);

    StudentNode* classmateRemove(int id);
    StudentNode* classmateRemoveFront(IdHashNode* idHash);
    StudentNode* classmateRemoveAfter(StudentNode* preNode);
    bool shouldRemoveHashNode(IdHashNode* idHash);

    //원형 더블 연결 처리
    void teammateInsert(StudentNode* newNode);
    void teammateInsertFirst(GroupHashNode* groupHash, StudentNode* newNode);
    void teammateInsertAfter(GroupHashNode* groupHash, StudentNode* newNode, StudentNode* preNode);
    void teammateRemove(StudentNode* targetNode);

    //소멸자 재귀문
    void deleteAllStudent(StudentNode* node);
};