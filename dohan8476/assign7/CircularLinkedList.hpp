#pragma once
#include "DoubliyNode.h"

template <typename T>
class CircularLinkedList {
    public:
        CircularLinkedList();
        ~CircularLinkedList();
        void add(T element);
        void append(T element);
        void insert(int index, T element);
        void remove(T element);
        void clear();
        int getSize() const;
        bool isEmpty() const;
        T get(int index) const;
        T getHead() const;
    private:
        DoublyNode<T> head;
        int size = 0;
};
