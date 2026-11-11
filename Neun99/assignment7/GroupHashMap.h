#pragma once
#include "GroupHashNode.h"

//GroupHashNode의 ArrayList
//IdHashMap처럼 따로 해싱해서 O(1)은 안되자만, 그룹 head 조정시 탐색이 필요하므로 정렬상태를 유지해서 이진탐색 사용
class GroupHashMap {
public:
    //생성자
    GroupHashMap();
    GroupHashMap(int minLength);
    ~GroupHashMap();

    //getter
    StudentNode* getHeadByKey(MyString group);
private:
    static constexpr int DEFAULT_CAPACITY = 4;
    int capacity;

};