#ifndef DOUBLENODE_H
#define DOUBLENODE_H

#include <utility>

#include "OldNode.h"

template <typename T>
class DoubleNode : public OldNode<T> {

    DoubleNode *prevNode = nullptr;
    DoubleNode *nextNode = nullptr;

public:

    DoubleNode(const T &item, DoubleNode *prevNode, DoubleNode *nextNode);

    DoubleNode(const T &item);

    DoubleNode(const DoubleNode &other) = delete;

    DoubleNode(DoubleNode &&other) noexcept;

    ~DoubleNode() override = default;

    DoubleNode &operator=(const DoubleNode &other) = delete;

    DoubleNode &operator=(DoubleNode &&other) noexcept;

    DoubleNode *getPrev() const;

    void setPrev(DoubleNode *prevNode);

    DoubleNode *getNext() const;

    void setNext(DoubleNode *nextNode);
    
};

#include "DoubleNode.tpp"
#endif // DOUBLENODE_H
