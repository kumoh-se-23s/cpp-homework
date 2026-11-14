#pragma once
#include "DoubleNode.h"

template<typename T>
DoubleNode<T> & DoubleNode<T>::operator=(const DoubleNode &other) {
    if (this == &other)
        return *this;
    item = other.item;
    prevNode = other.prevNode;
    nextNode = other.nextNode;
    return *this;
}

template<typename T>
DoubleNode<T> & DoubleNode<T>::operator=(DoubleNode &&other) noexcept {
    if (this == &other)
        return *this;
    item = std::move(other.item);
    prevNode = other.prevNode;
    nextNode = other.nextNode;
    return *this;
}

template<typename T>
T DoubleNode<T>::getItem() const {
    return item;
}

template<typename T>
void DoubleNode<T>::setItem(const T &item) {
    this->item = item;
}

template<typename T>
DoubleNode<T> * DoubleNode<T>::getPrevNode() const {
    return prevNode;
}

template<typename T>
void DoubleNode<T>::setPrevNode(DoubleNode<T> *prevNode) {
    this->prevNode = prevNode;
}

template<typename T>
DoubleNode<T> * DoubleNode<T>::getNextNode() const {
    return nextNode;
}

template<typename T>
void DoubleNode<T>::setNextNode(DoubleNode<T> *nextNode) {
    this->nextNode = nextNode;
}
