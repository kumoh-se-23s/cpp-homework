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
MyString NestedDoubleNode<T>::toString() const {
    if (node == nullptr) {
        return MyString("");
    }
    MyString result = MyString();

    result += node->toString();

    const DoubleNode<T> *otherDoubleNode = node->getNext();

    while (otherDoubleNode != node && otherDoubleNode != nullptr) {
        result +=" ";
        result += otherDoubleNode->toString();
        otherDoubleNode = otherDoubleNode->getNext();

        if (otherDoubleNode != node) {
            result += " -";
        }
    }
    return result;

}

template<typename T>
bool NestedDoubleNode<T>::operator<=(const NestedDoubleNode &otherNode) const {
    return this->node->getItem() <= otherNode.node->getItem();
}

