#pragma once
#include "MyString.h"
#include "StudentNode.h"
#include "IdHashMap.h"
#include "GroupHashMap.h"

class StudentMultiList {
public:
    StudentMultiList();
    StudentMultiList& operator=(const StudentMultiList& list);
    ~StudentMultiList();

    void presetCapacities(int yearCnt, int groupCnt);
    MyString toStringForSave() const;
    MyString toStringForPrint() const;
    bool insertStudent(int id, const MyString& group, const MyString& name);
    bool removeStudent(int id);
private:
    int studentCnt;
    IdHashMap idMap;
    GroupHashMap groupMap;
    const MyString NO_GROUP = "-";

    static int getYearOfId(int id);

    //단일 연결 처리
    bool classmateInsert(StudentNode* newNode);
    static void classmateInsertFront(IdHashNode* idHash, StudentNode* newNode);
    static void classmateInsertAfter(StudentNode* newNode, StudentNode* preNode);

    StudentNode* classmateRemove(int id);
    static StudentNode* classmateRemoveFront(IdHashNode* idHash);
    static StudentNode* classmateRemoveAfter(StudentNode* preNode);
    bool shouldRemoveHashNode(const IdHashNode * idHash) const;

    //원형 더블 연결 처리
    void teammateInsert(StudentNode* newNode);
    static void teammateInsertFirst(GroupHashNode* groupHash, StudentNode* newNode);
    static void teammateInsertAfter(StudentNode *newNode, StudentNode *preNode);
    void teammateRemove(const StudentNode * targetNode);

    // 소멸자 재귀문
    static void deleteAllStudent(const StudentNode * node);
};