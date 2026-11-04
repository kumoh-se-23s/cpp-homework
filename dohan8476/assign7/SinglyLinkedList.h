#pragma once

template <typename T>
class SinglyLinkedList {
    public:
        SinglyLinkedList();
        ~SinglyLinkedList();
        void append(T element);
        void insert(int index, T element);
        void remove(T element);
        void clear();
        int getSize() const;
        bool isEmpty() const;
        T get(int index) const;
        T getHead() const;

    private:
        struct Node {
            T data;
            Node* next;
            Node(T element) : data(element), next(nullptr) {}
        };
        Node* head;
        int size = 0;

        void insertFirst(T element);
        void removeFirst();
};

#include "SinglyLinkedList.tpp"
