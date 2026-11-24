#pragma once

template<typename T>
class NestedDoubleNode;


template<typename T>
const T& NestedDoubleNode<T>::getUnravelItem() const {
    return this->node->getItem();
}
template<typename T>
bool NestedDoubleNode<T>::operator==(const NestedDoubleNode &otherNode) const {
    return this->node->getItem() == otherNode.node->getItem();
}

template<typename T>
bool NestedDoubleNode<T>::operator!=(const NestedDoubleNode &otherNode) const {
    return this->node->getItem() != otherNode.node->getItem();
}

template<typename T>
bool NestedDoubleNode<T>::operator<(const NestedDoubleNode &otherNode) const {
    return this->node->getItem() < otherNode.node->getItem();
}

template<typename T>
bool NestedDoubleNode<T>::operator>(const NestedDoubleNode &otherNode) const {
    return this->node->getItem() > otherNode.node->getItem();
}

template<typename T>
bool NestedDoubleNode<T>::operator>=(const NestedDoubleNode &otherNode) const {
    return this->node->getItem() >= otherNode.node->getItem();
}
template<typename T>
bool NestedDoubleNode<T>::operator<=(const NestedDoubleNode &otherNode) const {
    return this->node->getItem() <= otherNode.node->getItem();
}
template<typename T>
OOldMyString NestedDoubleNode<T>::toString() const {
    if (node == nullptr) {
        return OOldMyString("");
    }
    return node->toString();
}

template<typename T>
const DoubleNode<T> * NestedDoubleNode<T>::getNode() const {
    return node;
}

