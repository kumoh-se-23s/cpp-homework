#pragma once

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
        struct DoublyNode{
            T data;
            DoublyNode* next;
            DoublyNode* prev;
            DoublyNode(T element) : data(element), next(nullptr), prev(nullptr) {}
        };
        DoublyNode* head;
        int size = 0;
};

#include "CircularLinkedList.tpp"