#pragma once

template<typename T>
class Node;

template<typename T>
Node<T> &Node<T>::operator=(const Node &other) {
    if (this != &other) {
        this->item = other.getItem();
    }

    return *this;
}

template<typename T>
Node<T> & Node<T>::operator=(Node &&other) noexcept {
    if (this != &other) {
        this->item = std::move(other.item);
    }
    return *this;
}

template<typename T>
const T & Node<T>::getItem() const {
    return item;
}

template<typename T>
T & Node<T>::getItem() {
    return item;
}

template<typename T>
void Node<T>::setItem(const T &item) {
    this->item = item;
}
template <typename T>
    MyString Node<T>::toString() const requires HasToString<T> {
    return item.toString();
}