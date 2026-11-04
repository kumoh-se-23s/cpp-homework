#pragma once
#include "SinglyLinkedList.h"


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() {
    head(nullptr);
    size = 0;
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {

}

template<typename T>
void SinglyLinkedList<T>::append(T element) {

}

template<typename T>
void SinglyLinkedList<T>::insert(int index, T element) {

}

template<typename T>
void SinglyLinkedList<T>::insertFirst(T element) {

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
void SinglyLinkedList<T>::removeFirst() {

}

template<typename T>
void SinglyLinkedList<T>::clear() {

}

