#ifndef NESTEDNODE_H
#define NESTEDNODE_H
#include <iostream>
#include "DoubleNode.h"

template<typename T>
class NestedDoubleNode {
    DoubleNode<T> *node;

public:
    NestedDoubleNode(DoubleNode<T> *item) : node(item) {
    }
    const DoubleNode<T>* getNode() const;
    const T &getUnravelItem() const;

    bool operator==(const NestedDoubleNode &otherNode) const;

    bool operator!=(const NestedDoubleNode &otherNode) const;

    bool operator<(const NestedDoubleNode &otherNode) const;

    bool operator>(const NestedDoubleNode &otherNode) const;

    bool operator<=(const NestedDoubleNode &otherNode) const;

    bool operator>=(const NestedDoubleNode &otherNode) const;

    OOldMyString toString() const;

};


#include "NestedDoubleNode.tpp"
#endif
