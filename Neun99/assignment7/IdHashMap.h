#pragma once
#include "IdHashNode.h"

//IdHashNode의 ArrayList
//학생 생성 및 삭제에서 학번을 통한 탐색이 자주 일어나는 점을 고려, 순서 정렬 시 원하는 년도에 O(1)로 접근할 수 있게 ArrayList 사용
//학번별 학생을 담는 자료구조이므로 희소배열이 나타나는 일은 극히 드물 거라 생각, index hash를 위해 내용이 비었더라도 최소~최대 사이의 모든 년도를 가짐
class IdHashMap {
public:
    //생성자
    IdHashMap();
    IdHashMap(int minLength);
    IdHashMap(const IdHashMap& idHashMap);
    ~IdHashMap();

    //getter
    int getMinYear() const;
    int getMaxYear() const;

    //기능
    IdHashNode* operator[](int year) const;
    IdHashNode* insert(int year);
    void remove(int year);
private:
    static constexpr int DEFAULT_CAPACITY = 4;
    int capacity = DEFAULT_CAPACITY;
    IdHashNode* arr;
    int size;

    void resize(int minLength);
    int getEnoughCapacity(int minLength) const;
    static void copyArr(IdHashNode* copy, IdHashNode* src, int srcSize);

    void insertFront(int year, int diff);
    void append(int year, int diff);
};