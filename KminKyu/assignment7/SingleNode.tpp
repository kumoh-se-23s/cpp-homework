#pragma once

template<typename T>
class SingleNode;

template<typename T>
SingleNode<T>::SingleNode(const T &item, SingleNode *nextNode)
    : Node<T>(item), nextNode(nextNode) {}

template<typename T>
SingleNode<T>::SingleNode(const T &item)
    : Node<T>(item) {}

template<typename T>
SingleNode<T>::SingleNode(SingleNode &&other) noexcept
    : Node<T>(std::move(other)) {
    this->nextNode = other.nextNode;
    other.nextNode = nullptr;
}

template <typename T>
SingleNode<T>& SingleNode<T>::operator=(SingleNode &&other) noexcept {
    if (this == &other)
        return *this;
    this->item = std::move(other.item);
    nextNode = other.nextNode;
    return *this;
}

template<typename T>
SingleNode<T> * SingleNode<T>::getNext() const {
    return nextNode;
}

template<typename T>
void SingleNode<T>::setNext(SingleNode<T> *next) {
    this->nextNode = next;
}




