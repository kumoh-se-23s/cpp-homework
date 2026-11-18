#pragma once

template<typename T>
class DoubleNode;

template<typename T>
DoubleNode<T>::DoubleNode(const T &item, DoubleNode *prevNode, DoubleNode *nextNode)
    :Node<T>(item), prevNode(prevNode), nextNode(nextNode) {}

template<typename T>
DoubleNode<T>::DoubleNode(const T &item) : DoubleNode(item, nullptr, nullptr) {}

template<typename T>
DoubleNode<T>::DoubleNode(const DoubleNode &other) : DoubleNode(other.item, other.prevNode, other.nextNode) {}

template<typename T>
DoubleNode<T>::DoubleNode(DoubleNode &&other) noexcept : Node<T>() {
    std::swap(this->item, other.item);
    this->nextNode = other.nextNode;
    this->prevNode = other.prevNode;
    other.nextNode = nullptr;
    other.prevNode = nullptr;
}

template<typename T>
DoubleNode<T> & DoubleNode<T>::operator=(const DoubleNode &other) {
    if (this == &other) {
        return *this;
    }
    this->item = other.item;
    prevNode = other.prevNode;
    nextNode = other.nextNode;

    return *this;
}

template<typename T>
DoubleNode<T> & DoubleNode<T>::operator=(DoubleNode &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    swap(this->item, other.item);
    prevNode = other.prevNode;
    nextNode = other.nextNode;
    return *this;
}

template<typename T>
const DoubleNode<T> *DoubleNode<T>::getPrevNode() const {
    return prevNode;
}

template<typename T>
void DoubleNode<T>::setPrevNode(DoubleNode *prevNode) {
    this->prevNode = prevNode;
}

template<typename T>
const DoubleNode<T> *DoubleNode<T>::getNextNode() const {
    return nextNode;
}

template<typename T>
void DoubleNode<T>::setNextNode(DoubleNode *nextNode) {
    this->nextNode = nextNode;
}
