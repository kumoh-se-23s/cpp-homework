#pragma once

template <typename T>
class SinglyLinkedList {
    public:
        SinglyLinkedList();
        ~SinglyLinkedList();
        void add(T element);
        void insert(int index, T element);
        void remove(int index);
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

        void removeFirst();
};

#include "SinglyLinkedList.tpp"
