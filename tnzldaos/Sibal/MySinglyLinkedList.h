#pragma once
#include <string>

#include "Comparator.h"
using namespace std;

//학번은 세로로 -> Year(다음 연도 대표), nextId(같은 연도 다음 학생) 사용
//이 리스트가 노드의 주인 -> 소멸될 때 가진 노드를 모두 delete
template<typename T>
class MySinglyLinkedList {
    T *head = nullptr;

public:
    MySinglyLinkedList() = default;

    ~MySinglyLinkedList();

    //복사하면 주인이 둘이 되어 같은 노드를 두 번 지우게 되므로 복사 금지
    MySinglyLinkedList(const MySinglyLinkedList &) = delete;

    MySinglyLinkedList &operator=(const MySinglyLinkedList &) = delete;

    explicit MySinglyLinkedList(T *inputNode);

    T *getHead() const;

    //넣었으면 true, 같은 값이 이미 있어서 못 넣었으면 false
    template<typename U>
    bool sortedInsert(T *input, Comparator<U> &comparator);

    //리스트에서 빼낸 노드를 돌려줌 -> 이제 지우는 책임은 받은 쪽에 있음
    T *remove(const string &input);
};

#include "MySinglyLinkedList.tpp"
