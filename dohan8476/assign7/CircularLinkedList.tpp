#pragma once
#include "CircularLinkedList.h"

template <typename T>
CircularLinkedList<T>::CircularLinkedList() {
    head = nullptr;
    size = 0;
}

template <typename T>
CircularLinkedList<T>::~CircularLinkedList() {

}

template<typename T>
void CircularLinkedList<T>::add(T element) {
    Node newNode = new Node(element);
    if (isEmpty()) {
        head = newNode;
    }
}

template<typename T>
void CircularLinkedList<T>::append(T element) {

}

template<typename T>
void CircularLinkedList<T>::insert(int index, T element) {

}

template<typename T>
void CircularLinkedList<T>::remove(T element) {

}

template<typename T>
void CircularLinkedList<T>::remove(int index) {

}

template<typename T>
void CircularLinkedList<T>::clear() {

}

template<typename T>
int CircularLinkedList<T>::getSize() const{
    return size;
}

template<typename T>
bool CircularLinkedList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
T CircularLinkedList<T>::get(int index) const {

}

template<typename T>
T CircularLinkedList<T>::getHead() const {

}

template<typename T>
void CircularLinkedList<T>::removeFirst() {

}
