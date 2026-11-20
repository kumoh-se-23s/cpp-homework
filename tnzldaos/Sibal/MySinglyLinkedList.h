#pragma once
#include <string>

#include "Comparator.h"
using namespace std;

//학번은 세로로 -> Year(다음 연도 대표), nextId(같은 연도 다음 학생) 사용
template<typename T>
class MySinglyLinkedList {
    T *head = nullptr;

public:
    MySinglyLinkedList() = default;

    ~MySinglyLinkedList() = default;

    explicit MySinglyLinkedList(T *inputNode);

    T *getHead() const;

    template<typename U>
    void sortedInsert(T *input, Comparator<U> &comparator);

    T *remove(const string &input);
};

#include "MySinglyLinkedList.tpp"
