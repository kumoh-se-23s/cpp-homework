#pragma once
#include "GroupHashNode.h"

//GroupHashNode의 ArrayList
//IdHashMap처럼 따로 해싱해서 O(1)은 안되지만 그룹 head 조정시 탐색이 필요하므로 정렬상태를 유지해서 이진탐색 사용
class GroupHashMap {
public:
    //생성자
    GroupHashMap();
    GroupHashMap(int minLength);
    GroupHashMap(const GroupHashMap& groupHashMap);
    GroupHashMap& operator=(const GroupHashMap& groupHashMap);
    ~GroupHashMap();

    //getter
    GroupHashNode* operator[](const MyString& group) const;
    int getSize() const;

    //기능
    GroupHashNode* insert(const MyString& group);
    void remove(const MyString& group);
private:
    static constexpr int DEFAULT_CAPACITY = 4;
    int capacity = DEFAULT_CAPACITY;
    GroupHashNode *arr;
    int size;

    void resize(int minLength);
    int getEnoughCapacity(int minLength) const;
    static void copyArr(GroupHashNode* copy, GroupHashNode* src, int srcSize);

    void insertAt(const MyString& group, int pos);
    int getTargetPos(const MyString& group) const;
};