#pragma once

template<typename T>
class OldSingleNode;

template<typename T>
OldSingleNode<T>::OldSingleNode(const T &item, OldSingleNode *nextNode)
    : OldNode<T>(item), nextNode(nextNode) {}

template<typename T>
OldSingleNode<T>::OldSingleNode(const T &item)
    : OldNode<T>(item) {}

template<typename T>
OldSingleNode<T>::OldSingleNode(OldSingleNode &&other) noexcept
    : OldNode<T>(std::move(other)) {
    this->nextNode = other.nextNode;
    other.nextNode = nullptr;
}

template <typename T>
OldSingleNode<T>& OldSingleNode<T>::operator=(OldSingleNode &&other) noexcept {
    if (this == &other)
        return *this;
    this->item = std::move(other.item);

    nextNode = other.nextNode;
    other.nextNode = nullptr;

    return *this;
}

template<typename T>
OldSingleNode<T> * OldSingleNode<T>::getNext() const {
    return nextNode;
}

template<typename T>
void OldSingleNode<T>::setNext(OldSingleNode<T> *next) {
    this->nextNode = next;
}




