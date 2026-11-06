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
        void remove(int index);
        void clear();
        int getSize() const;
        bool isEmpty() const;
        T get(int index) const;
        T getHead() const;
    private:
        struct Node{
            T data;
            Node* next;
            Node* prev;
            Node(T element) : data(element), next(nullptr), prev(nullptr) {}
        };
        Node* head;
        int size = 0;

        void removeFirst();
};

#include "CircularLinkedList.tpp"