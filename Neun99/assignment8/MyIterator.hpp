#pragma once
#include "MyIterator.h"

template<typename T>
MyIterator<T>::MyIterator(const Node<T>* head) {
    cursor = head;
}

template<typename T>
const T &MyIterator<T>::next(){
    cursor = cursor->getNextNode();
    return cursor->getItem();
}

template<typename T>
bool MyIterator<T>::hasNext() const {
    return cursor->getNextNode() != nullptr;
}