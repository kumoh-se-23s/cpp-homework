#ifndef CIRCULARDOUBLYLINKEDLIST_H
#define CIRCULARDOUBLYLINKEDLIST_H
#include "DoubleNode.h"

template <typename T>
class CircularDoublyLinkedList {
    DoubleNode<T> *head = nullptr;
    int size = 0;
public:
    CircularDoublyLinkedList() = default;

    CircularDoublyLinkedList(DoubleNode<T> *head);

    ~CircularDoublyLinkedList();

    CircularDoublyLinkedList(const CircularDoublyLinkedList &singlyLinkedList) = delete;

    CircularDoublyLinkedList(CircularDoublyLinkedList &&singlyLinkedList) noexcept;

    CircularDoublyLinkedList &operator=(const CircularDoublyLinkedList &otherLinkedList) = delete;

    CircularDoublyLinkedList &operator=(CircularDoublyLinkedList &&otherLinkedList) noexcept;

    DoubleNode<T> *getHead();

    int getSize();

    const DoubleNode<T> *addFirst(T item);

    const DoubleNode<T> *add(T item);

    DoubleNode<T> *find(T item);

    bool remove(T item);

    bool removeFirst(T item);

    void clear();
};

#include "CircularDoublyLinkedList.tpp"
#endif // CIRCULARDOUBLYLINKEDLIST_H
