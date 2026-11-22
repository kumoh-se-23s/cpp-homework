#ifndef SINGLENODE_H
#define SINGLENODE_H

#include <iostream>
#include <utility>

#include "MyString.h"
#include "Node.h"


template <typename T>
class SingleNode : public Node<T>{

    SingleNode *nextNode = nullptr;
public:
    SingleNode() = default;

    SingleNode(const T &item, SingleNode *nextNode);

    SingleNode(const T &item);

    SingleNode(const SingleNode &other) = delete;

    SingleNode(SingleNode &&other) noexcept;

    ~SingleNode() override = default;

    SingleNode &operator=(const SingleNode &other) = delete;

    SingleNode &operator=(SingleNode &&other) noexcept;

    SingleNode *getNext() const;

    void setNext(SingleNode *next);

};

#include "SingleNode.tpp"
#endif