#include "GroupHashMap.h"

GroupHashMap::GroupHashMap() {
    arr = new GroupHashNode[capacity];
    size = 0;
}

GroupHashMap::GroupHashMap(int minLength) {
    arr = new GroupHashNode[getEnoughCapacity(minLength)];
    size = 0;
}

GroupHashMap::~GroupHashMap() {

}

//연산자오버로딩-----------------------
//인자로 받은 그룹의 head 꺼내기: 없으면 위치맞춰 만들어서 넘겨줌
StudentNode *GroupHashMap::operator[](MyString group) {
    int pos = getTargetPos(group);
    if (arr[pos].getId() != group)
        insertAt(group, pos);
    return arr[pos].getHead();
}


//private-----------------------------
void GroupHashMap::resize(int minLength) {
    capacity = getEnoughCapacity(minLength);
    GroupHashNode* newArr = new GroupHashNode[capacity];
    copyArr(newArr, arr, size);
    delete[] arr;
    arr = newArr;
}

int GroupHashMap::getEnoughCapacity(int minLength) const {
    int result;
    for (result = capacity; result > minLength; result *= 2) {}
    return result;
}

void GroupHashMap::copyArr(GroupHashNode *copy, GroupHashNode *src, int srcSize) {
    for (int idx = 0; idx < srcSize; idx++) {
        copy[idx] = src[idx];
    }
}

void GroupHashMap::insertAt(MyString group, int pos) {
    //용량검사
    if (size + 1 > capacity) {
        GroupHashNode* newArr = new GroupHashNode[getEnoughCapacity(size + 1)];

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
    } else { //용량 괜찮으면
        //뒷부분 밀기
        for (int idx = size - 1; idx >= pos; idx--) {
            arr[idx + 1] = arr[idx];
        }

        //insert
        arr[pos] = GroupHashNode(group);
    }
}

//이진탐색기반 target 위치찾기
int GroupHashMap::getTargetPos(MyString group) const{
    int lowIdx = 0;
    int highIdx = size - 1;
    while (lowIdx <= highIdx) {
        int midIdx = lowIdx + ((highIdx - lowIdx) / 2);
        if (arr[midIdx].getId() == group)
            return -1;
        if (arr[midIdx].getId() < group)
            lowIdx = midIdx + 1;
        else
            highIdx = midIdx - 1;
    }
    return lowIdx;
}
