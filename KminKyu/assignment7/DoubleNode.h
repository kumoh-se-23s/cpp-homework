#ifndef DOUBLENODE_H
#define DOUBLENODE_H

#include <utility>
#include "Node.h"

template <typename T>
class DoubleNode : public Node<T> {
    T item;
    DoubleNode<T> *prevNode = nullptr;
    DoubleNode<T> *nextNode = nullptr;
public:
    DoubleNode(const T &item, DoubleNode<T> *prevNode, DoubleNode<T> *nextNode);

    DoubleNode(const DoubleNode &other);

    DoubleNode(DoubleNode &&other) noexcept;

    DoubleNode & operator=(const DoubleNode &other);

    DoubleNode & operator=(DoubleNode &&other) noexcept;

    ~DoubleNode() = default;

    T getItem() const;

    void setItem(const T &item);

    DoubleNode<T> * getPrevNode() const;

    void setPrevNode(DoubleNode<T> *prevNode);

    DoubleNode<T> * getNextNode() const;

    void setNextNode(DoubleNode<T> *nextNode);

};

#include "DoubleNode.tpp"

#endif // DOUBLENODE_H
