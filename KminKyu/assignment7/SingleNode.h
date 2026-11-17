#ifndef SINGLENODE_H
#define SINGLENODE_H

#include "Node.h"

template <typename T>
class SingleNode : public Node<T> {
    T item;
    SingleNode<T> *nextNode = nullptr;
public:
    SingleNode(const T &item, SingleNode<T> *nextNode);

    SingleNode(const SingleNode &other);

    SingleNode(SingleNode &&other) noexcept;

    SingleNode &operator=(const SingleNode &other);

    SingleNode &operator=(SingleNode &&other) noexcept;

    ~SingleNode() override;

    const T& getItem() const;

    void setItem(const T &item);

    SingleNode<T> *getNext() const;
    void setNext(SingleNode<T> *next);


};

#include "SingleNode.tpp"
#endif