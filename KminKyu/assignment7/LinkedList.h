#ifndef CPP_HOMEWORK_GIT_LINKEDLIST_H
#define CPP_HOMEWORK_GIT_LINKEDLIST_H

template <typename T>
class LinkedList {
public:
    LinkedList() = default;

    virtual ~LinkedList() = 0;

    virtual void addFirst(T item) = 0;

    virtual void add(T item) = 0;

    virtual Node<T>& find(T item) = 0;

    virtual bool remove(T) = 0;

    virtual bool removeFirst(T) = 0;

};

#endif
