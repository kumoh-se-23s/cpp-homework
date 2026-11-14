#pragma once
#include "DoubleNode.h"

template <typename T>
class CircularDoublyLinkedList {
    T key;
    DoubleNode<T> headNode;
    int size = 0;
};


