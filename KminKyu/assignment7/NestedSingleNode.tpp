#pragma once

template<typename T>
class NestedSingleNode;

template<typename T>
const T& NestedSingleNode<T>::getUnravelItem() {
    return this->item.getItem();
}
template<typename T>
bool NestedSingleNode<T>::operator==(const NestedSingleNode &otherNode) const {
    return this->item.getItem() == otherNode.item.getItem();
}

template<typename T>
bool NestedSingleNode<T>::operator!=(const NestedSingleNode &otherNode) const {
    return this->item.getItem() != otherNode.item.getItem();
}

template<typename T>
bool NestedSingleNode<T>::operator<(const NestedSingleNode &otherNode) const {
    return this->item.getItem() < otherNode.item.getItem();
}

template<typename T>
bool NestedSingleNode<T>::operator>(const NestedSingleNode &otherNode) const {
    return this->item.getItem() > otherNode.item.getItem();
}

template<typename T>
bool NestedSingleNode<T>::operator>=(const NestedSingleNode &otherNode) const {
    return this->item.getItem() >= otherNode.item.getItem();
}

template<typename T>
bool NestedSingleNode<T>::operator<=(const NestedSingleNode &otherNode) const {
    return this->item.getItem() <= otherNode.item.getItem();
}
