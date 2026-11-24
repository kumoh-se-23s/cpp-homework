#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <iostream>
#include <utility>

#include "SingleNode.h"

template <typename T>
class SinglyLinkedList {
    SingleNode<T> *head = nullptr;
    int size = 0;
public:
    SinglyLinkedList() = default;

    SinglyLinkedList(SingleNode<T> *head);

    ~SinglyLinkedList();

    SinglyLinkedList(const SinglyLinkedList &singlyLinkedList) = delete;

    SinglyLinkedList(SinglyLinkedList &&singlyLinkedList) noexcept;

    SinglyLinkedList &operator=(const SinglyLinkedList &otherLinkedList) = delete;

    SinglyLinkedList &operator=(SinglyLinkedList &&otherLinkedList) noexcept;

    SingleNode<T> *getHead() const;

    int getSize() const;

    SingleNode<T> *add(SingleNode<T> *node);

    SingleNode<T> *add(const T& item);

    const SingleNode<T> *find(const T& item) const;

    SingleNode<T> *find(const T& item);

    bool remove(const T& item);

    void clear();

    MyString toString() const;
private:

    SingleNode<T> *addFirst(SingleNode<T> *node);

    //정렬되지 않은 여러개의 노드가 들어오는것을 가정
    SingleNode<T> *addAll(SingleNode<T> *node);

    SingleNode<T> *addNode(SingleNode<T> *node);

    bool removeFirst(const T& item);

};

template <typename T>
std::ostream& operator<<(std::ostream& out, const SinglyLinkedList<T>& singlyLinkedList);

template <typename T>
std::ofstream& operator<<(std::ofstream& out, const SinglyLinkedList<T>& singlyLinkedList);

#include "SinglyLinkedList.tpp"
#endif // SINGLYLINKEDLIST_H