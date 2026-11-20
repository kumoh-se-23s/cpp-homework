#include "GroupHashMap.h"

GroupHashMap::GroupHashMap() {
    arr = new GroupHashNode[capacity];
    size = 0;
}

//복사생성자
GroupHashMap::GroupHashMap(const GroupHashMap& groupHashMap) {
    size = groupHashMap.size;
    capacity = getEnoughCapacity(size);
    arr = new GroupHashNode[capacity];
    for (int idx = 0; idx < size; idx++) {
        arr[idx] = groupHashMap.arr[idx];
    }
}

//대입연산자
GroupHashMap &GroupHashMap::operator=(const GroupHashMap &groupHashMap) {
    if (this != &groupHashMap) {
        size = groupHashMap.size;
        capacity = getEnoughCapacity(size);
        GroupHashNode* newArr = new GroupHashNode[capacity];
        for (int idx = 0; idx < size; idx++) {
            newArr[idx] = groupHashMap.arr[idx];
        }
        delete[] arr;
        arr = newArr;
    }

    return *this;
}

//소멸자
GroupHashMap::~GroupHashMap() {
    delete[] arr;
}

//getter
//인자로 받은 그룹의 노드 꺼내기: 없으면 nullptr
GroupHashNode *GroupHashMap::operator[](const MyString &group) const {
    int pos = getTargetPos(group);
    if (pos >= size || arr[pos].getId() != group)
        return nullptr;
    return &arr[pos];
}

//size 반환
int GroupHashMap::getSize() const {
    return size;
}

//기능-----------------------
//용량 미리 확장
void GroupHashMap::presetCapacity(int minLength) {
    capacity = getEnoughCapacity(minLength);
    GroupHashNode* newArr = new GroupHashNode[capacity];
    copyArr(newArr, arr, size);
    size = 0;
}

// 인자로 받은 그룹 없으면 위치맞춰 만들어서 넘겨줌
GroupHashNode *GroupHashMap::insert(const MyString &group) {
    int pos = getTargetPos(group);
    if (pos >= size || arr[pos].getId() != group)
        insertAt(group, pos);
    return &arr[pos];
}

void GroupHashMap::remove(const MyString &group) {
    int pos = getTargetPos(group);
    if (pos < size && arr[pos].getId() == group) {
        for (int idx = pos; idx < size - 1; idx++) {
            arr[idx] = arr[idx + 1];
        }
        size--;
    }
}

//private-----------------------------
int GroupHashMap::getEnoughCapacity(int minLength) const {
    int result;
    for (result = capacity; result < minLength; result *= 2) {}
    return result;
}

void GroupHashMap::copyArr(GroupHashNode *copy, GroupHashNode *src, int srcSize) {
    for (int idx = 0; idx < srcSize; idx++) {
        copy[idx] = src[idx];
    }
}

void GroupHashMap::insertAt(const MyString &group, int pos) {
    //용량검사
    if (size + 1 > capacity) { //용량 안 괜찮으면
        //새로 할당
        capacity = getEnoughCapacity(size + 1);
        GroupHashNode* newArr = new GroupHashNode[capacity];

        int idx;
        //앞부분 복사
        for (idx = 0; idx < pos; idx++) {
            newArr[idx] = arr[idx];
        }

        //insert
        newArr[idx] = GroupHashNode(group);

        //뒷부분 복사
        for (; idx < size; idx++) {
            newArr[idx + 1] = arr[idx];
        }

        delete[] arr;
        arr = newArr;
    } else { //용량 괜찮으면
        //뒷부분 밀기
        for (int idx = size - 1; idx >= pos; idx--) {
            arr[idx + 1] = arr[idx];
        }

        //insert
        arr[pos] = GroupHashNode(group);
    }
    size++;
}

//이진탐색기반 target 위치찾기
int GroupHashMap::getTargetPos(const MyString &group) const{
    int lowIdx = 0;
    int highIdx = size - 1;
    while (lowIdx <= highIdx) {
        int midIdx = lowIdx + ((highIdx - lowIdx) / 2);
        if (arr[midIdx].getId() == group)
            return midIdx;
        if (arr[midIdx].getId() < group)
            lowIdx = midIdx + 1;
        else
            highIdx = midIdx - 1;
    }
    return lowIdx;
}