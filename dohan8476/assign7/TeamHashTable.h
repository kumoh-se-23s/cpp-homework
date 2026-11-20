#pragma once

#include "CircularLinkedList.h"
#include "StudentNode.h"

class TeamHashTable {
    //hash할 때 capacity써서 0이면 안됨
    static constexpr int DEFAULT_CAPACITY = 4;
    CircularLinkedList<StudentNode>* buckets;
    MyString* teamIDs;
    int capacity;
    int groupCount;

    public:
        TeamHashTable(int initCap = DEFAULT_CAPACITY);
        ~TeamHashTable();

        void insert(StudentNode* node);
        void remove(StudentNode* node);
        int getGroupCount() const;

    private:
        int hash(const MyString& teamID) const;
        int find(const MyString& teamID) const;
        int createBucket(const MyString& teamID);
        void resize();
};