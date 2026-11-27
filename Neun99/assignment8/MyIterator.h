#pragma once
#include "Node.h"

template<typename T>
class MyIterator {
public:
    MyIterator(const Node<T>* head);

    const T& next();
    bool hasNext() const;
private:
    Node<T>* cursor;
};