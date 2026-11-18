#ifndef SINGLENODE_H
#define SINGLENODE_H
#include "Node.h"


template <typename T>
class SingleNode : public Node<T>{

    SingleNode *nextNode = nullptr;
public:
    SingleNode(const T &item, SingleNode *nextNode);

    SingleNode(const T &item);

    SingleNode(const SingleNode &other);

    SingleNode(SingleNode &&other) noexcept;

    // ~SingleNode() = default;
    SingleNode &operator=(const SingleNode &other);

    SingleNode &operator=(SingleNode &&other) noexcept;

    const T& getItem() const override;

    void setItem(const T &item);

    SingleNode *getNext() const;

    void setNext(SingleNode *next);
};
#include "SingleNode.tpp"
#endif