#pragma once
#include "Node.h"

template<typename T>
Node<T>::Node(const T *item) {
    this->item = item;
    nextNode = nullptr;
}

template<typename T>
Node<T>::~Node() {
    delete item;
}

template<typename T>
T *Node<T>::getItem() const {
    return item;
}

template<typename T>
Node<T> *Node<T>::getNextNode() const {
    return nextNode;
}

template<typename T>
void Node<T>::setItem(const T *item) {
    this->item = item;
}

template<typename T>
void Node<T>::setNextNode(const Node *nextNode) {
    this->nextNode = nextNode;
}
