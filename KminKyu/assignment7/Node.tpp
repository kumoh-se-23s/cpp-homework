#pragma once

template<typename T>
class Node;

template<typename T>
const T & Node<T>::getItem() const {
    return item;
}

template<typename T>
T & Node<T>::getItem() {
    return item;
}

template<typename T>
void Node<T>::setItem(T &item) {
    this->item = item;
}