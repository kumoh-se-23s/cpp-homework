#pragma once

#include "SingleNode.h"

template<typename T>
SingleNode<T>::SingleNode(const T &item, SingleNode<T> *nextNode) {
    this->item = item;
    this->nextNode = nextNode;
}

template<typename T>
SingleNode<T>::SingleNode(const SingleNode &other) {
    this->item = other.item;
    this->nextNode = other.nextNode;
}

template<typename T>
SingleNode<T>::SingleNode(SingleNode &&other) noexcept {
    item = std::move(other.item);
    nextNode = other.nextNode;
    other.nextNode = nullptr;
}

template <typename T>
SingleNode<T>& SingleNode<T>::operator=(const SingleNode &other) {
    if (this == &other)
        return *this;
    item = other.item;
    nextNode = other.nextNode;
    return *this;
}

template <typename T>
SingleNode<T>& SingleNode<T>::operator=(SingleNode &&other) noexcept {
    if (this == &other)
        return *this;
    item = std::move(other.item);
    nextNode = other.nextNode;
    return *this;
}

template<typename T>
SingleNode<T>::~SingleNode() {
    delete item;
}

template<typename T>
SingleNode<T> * SingleNode<T>::getNext() const {
    return nextNode;
}

template<typename T>
void SingleNode<T>::setNext(SingleNode<T> *next) {
    this->nextNode = next;
}

template<typename T>
const T &SingleNode<T>::getItem() const {
    return item;
}

template<typename T>
void SingleNode<T>::setItem(const T &item) {
    this->item = item;
}

