#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <iostream>
#include <utility>

#include "OldSingleNode.h"

template <typename T>
class OldSinglyLinkedList {
    OldSingleNode<T> *head = nullptr;
    int size = 0;
public:
    OldSinglyLinkedList() = default;

    OldSinglyLinkedList(OldSingleNode<T> *head);

    ~OldSinglyLinkedList();

    OldSinglyLinkedList(const OldSinglyLinkedList &singlyLinkedList) = delete;

    OldSinglyLinkedList(OldSinglyLinkedList &&singlyLinkedList) noexcept;

    OldSinglyLinkedList &operator=(const OldSinglyLinkedList &otherLinkedList) = delete;

    OldSinglyLinkedList &operator=(OldSinglyLinkedList &&otherLinkedList) noexcept;

    OldSingleNode<T> *getHead() const;

    int getSize() const;

    OldSingleNode<T> *add(OldSingleNode<T> *node);

    OldSingleNode<T> *add(const T& item);

    const OldSingleNode<T> *find(const T& item) const;

    OldSingleNode<T> *find(const T& item);

    bool remove(const T& item);

    void clear();

    OOldMyString toString() const;
private:

    OldSingleNode<T> *addFirst(OldSingleNode<T> *node);

    //정렬되지 않은 여러개의 노드가 들어오는것을 가정
    OldSingleNode<T> *addAll(OldSingleNode<T> *node);

    OldSingleNode<T> *addNode(OldSingleNode<T> *node);

    bool removeFirst(const T& item);

};

template <typename T>
std::ostream& operator<<(std::ostream& out, const OldSinglyLinkedList<T>& singlyLinkedList);

#include "OldSinglyLinkedList.tpp"
#endif // SINGLYLINKEDLIST_H