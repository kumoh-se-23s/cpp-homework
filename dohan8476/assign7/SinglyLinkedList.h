#pragma once

template <typename T>
class SinglyLinkedList {
    public:
        SinglyLinkedList();
        ~SinglyLinkedList();

        void add(T* newNode);
        void insert(int index, T* newNode);
        void insertAscend(T* newNode);

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

        bool outOfRange(int index) const;
};

#include "SinglyLinkedList.tpp"
