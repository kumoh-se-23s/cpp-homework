#ifndef CPP_HOMEWORK_GIT_LINKEDLIST_H
#define CPP_HOMEWORK_GIT_LINKEDLIST_H

template <typename T>
class LinkedList {
    Node<T> head = nullptr;

    int size = 0;

public:
    LinkedList() = default;

    virtual ~LinkedList() = 0;

    const Node<T>& getHead() const;

    int getSize() const;

    void setHead(Node<T> head);

    virtual void append(Node<T> item) = 0;
    virtual Node<T>
};

#include "LinkedList.tpp"

#endif
