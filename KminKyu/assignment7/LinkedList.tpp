#pragma once
#include "LinkedList.h"

template<typename T>
const Node<T>& LinkedList<T>::getHead() const {
    return head;
}

template<typename T>
int LinkedList<T>::getSize() const {
    return size;
}

template<typename T>
void LinkedList<T>::setHead(Node<T> head) {
    this->head = head;
}

