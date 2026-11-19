#ifndef NESTEDNODE_H
#define NESTEDNODE_H
#include "DoubleNode.h"

template<typename T>
class NestedDoubleNode {
    DoubleNode<T> *node;

public:
    NestedDoubleNode(DoubleNode<T> *item) : node(item) {
    }

    const T &getUnravelItem() const;

    bool operator==(const NestedDoubleNode &otherNode) const;

    bool operator!=(const NestedDoubleNode &otherNode) const;

    bool operator<(const NestedDoubleNode &otherNode) const;

    bool operator>(const NestedDoubleNode &otherNode) const;

    bool operator<=(const NestedDoubleNode &otherNode) const;

    bool operator>=(const NestedDoubleNode &otherNode) const;

    MyString toString() const;

};

#include "NestedNode.tpp"
#endif
