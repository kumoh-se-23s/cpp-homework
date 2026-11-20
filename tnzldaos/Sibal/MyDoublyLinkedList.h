#pragma once

#include "Comparator.h"

//팀은 가로로 -> nextTeamMember, prevTeamMember
template<typename T>
class MyDoublyLinkedList {
    T *head = nullptr;

public:
    MyDoublyLinkedList() = default;

    ~MyDoublyLinkedList() = default;

    explicit MyDoublyLinkedList(T *inputNode);

    T *getHead() const;

    template<typename U>
    void sortedInsert(T *input, Comparator<U> &comparator);

    void remove(T *target);
};

#include "MyDoublyLinkedList.tpp"

