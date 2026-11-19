#pragma once

template<typename T>
class DoubleNode;

template<typename T>
DoubleNode<T>::DoubleNode(const T &item, DoubleNode *prevNode, DoubleNode *nextNode)
    :Node<T>(item), prevNode(prevNode), nextNode(nextNode) {}

template<typename T>
DoubleNode<T>::DoubleNode(const T &item) : DoubleNode(item, nullptr, nullptr) {}

template<typename T>
DoubleNode<T>::DoubleNode(DoubleNode &&other) noexcept : Node<T>(std::move(other)) {
    this->nextNode = other.nextNode;
    this->prevNode = other.prevNode;
    other.nextNode = nullptr;
    other.prevNode = nullptr;
}

template<typename T>
DoubleNode<T> & DoubleNode<T>::operator=(DoubleNode &&other) noexcept {
    if (this != &other) {
        this->item = std::move(other.item);
        prevNode = other.prevNode;
        nextNode = other.nextNode;

        other.prevNode = nullptr;
        other.nextNode = nullptr;

    }

    return *this;
}

template<typename T>
DoubleNode<T> *DoubleNode<T>::getPrev() const {
    return prevNode;
}

template<typename T>
void DoubleNode<T>::setPrev(DoubleNode *prevNode) {
    this->prevNode = prevNode;
}

template<typename T>
DoubleNode<T> *DoubleNode<T>::getNext() const {
    return nextNode;
}

template<typename T>
void DoubleNode<T>::setNext(DoubleNode *nextNode) {
    this->nextNode = nextNode;
}
