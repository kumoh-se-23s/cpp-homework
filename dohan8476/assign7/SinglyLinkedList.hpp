#pragma once
#include "SinglyNode.h"

template <typename T>
class SinglyLinkedList {
    public:
        SinglyLinkedList();
        ~SinglyLinkedList();
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
        SinglyHeadNode<T> head;
        int size = 0;
};

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() {

}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {

}

template<typename T>
void SinglyLinkedList<T>::add(T element) {

}

template<typename T>
void SinglyLinkedList<T>::append(T element) {

}

template<typename T>
void SinglyLinkedList<T>::insert(int index, T element) {

}

template<typename T>
T SinglyLinkedList<T>::get(int index) const {

}

template<typename T>
T SinglyLinkedList<T>::getHead() const {

}

template<typename T>
int SinglyLinkedList<T>::getSize() const {
    return size;
}

template<typename T>
bool SinglyLinkedList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void SinglyLinkedList<T>::remove(T element) {

}

template<typename T>
void SinglyLinkedList<T>::clear() {

}
