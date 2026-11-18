#ifndef DOUBLENODE_H
#define DOUBLENODE_H

#include <utility>
#include "Node.h"

template <typename T>
class DoubleNode : public Node<T> {

    DoubleNode *prevNode = nullptr;
    DoubleNode *nextNode = nullptr;

public:

    DoubleNode(const T &item, DoubleNode *prevNode, DoubleNode *nextNode);

    DoubleNode(const T &item);

    DoubleNode(const DoubleNode &other);

    DoubleNode(DoubleNode &&other) noexcept;

    ~DoubleNode() override = default;

    DoubleNode &operator=(const DoubleNode &other);

    DoubleNode &operator=(DoubleNode &&other) noexcept;

    const DoubleNode *getPrevNode() const;

    void setPrevNode(DoubleNode *prevNode);

    const DoubleNode *getNextNode() const;

    void setNextNode(DoubleNode *nextNode);
};

#include "DoubleNode.tpp"
#endif // DOUBLENODE_H
