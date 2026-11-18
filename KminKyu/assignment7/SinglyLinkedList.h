#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H
#include "SingleNode.h"

template <typename T>
class SinglyLinkedList {
    SingleNode<T> *head = nullptr;
    int size = 0;
public:
    SinglyLinkedList() = default;

    SinglyLinkedList(SingleNode<T> head);

    ~SinglyLinkedList();

    SinglyLinkedList(const SinglyLinkedList &singlyLinkedList) = delete;

    SinglyLinkedList(SinglyLinkedList &&singlyLinkedList) noexcept;

    SinglyLinkedList &operator=(const SinglyLinkedList &otherLinkedList) = delete;

    SinglyLinkedList &operator=(SinglyLinkedList &&otherLinkedList) noexcept;

    SingleNode<T> *getHead();

    int getSize();

    const SingleNode<T> *addFirst(T item);

    const SingleNode<T> *add(T item);

    SingleNode<T> *find(T item);

    bool remove(T item);

    bool removeFirst(T item);

    void clear();
};

#include "SinglyLinkedList.tpp"
#endif // SINGLYLINKEDLIST_H