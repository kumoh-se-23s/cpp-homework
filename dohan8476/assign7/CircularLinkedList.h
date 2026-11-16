#pragma once

template <typename T>
class CircularLinkedList {
    public:
        CircularLinkedList();
        ~CircularLinkedList();
        void add(T* newNode);
        void append(T* newNode);
        void insert(int index, T* newNode);
        void insertAscend(T* newNode);

        T* remove(T* target);
        template <typename KeyType>
        T* remove(const KeyType& element);
        T* remove(int index);
        T* removeFirst();

        int getSize() const;
        bool isEmpty() const;
        T* get(int index) const;
        T* getHead() const;
    private:
        T* head;
        int size;
};

#include "CircularLinkedList.tpp"