#pragma once
#include "SingleNode.h"

template<typename T>
SingleNode<T>::SingleNode(const T &item, SingleNode<T> *nextNode) {
}

template<typename T>
SingleNode<T>::SingleNode(const SingleNode &other) {
}

template<typename T>
SingleNode<T>::SingleNode(SingleNode &&other) noexcept {
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
const T &SingleNode<T>::getItem() const {
    return item;
}

template<typename T>
void SingleNode<T>::setItem(const T &item) {
    this->item = item;
}

