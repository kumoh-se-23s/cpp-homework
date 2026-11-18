#ifndef NESTEDSINGLENODE_H
#define NESTEDSINGLENODE_H
#include "SingleNode.h"
#include "Node.h"

template <typename T>
class NestedSingleNode : public SingleNode<Node<T>> {
public:
    ~NestedSingleNode() override = default;

    NestedSingleNode(const Node<T> &item)
        : SingleNode<Node<T>>(item, nullptr) {}

    NestedSingleNode(const Node<T> &item, SingleNode<Node<T>> *nextNode)
        : SingleNode<Node<T>>(item, nextNode) {}

    NestedSingleNode(const SingleNode<Node<T>> &other)
        : SingleNode<Node<T>>(other) {}

    NestedSingleNode(SingleNode<Node<T>> &&other)
        : SingleNode<Node<T>>(other) {}

    const T& getUnravelItem();

    bool operator==(const NestedSingleNode& otherNode) const;
    bool operator!=(const NestedSingleNode& otherNode) const;
    bool operator<(const NestedSingleNode& otherNode) const;
    bool operator>(const NestedSingleNode& otherNode) const;
    bool operator<=(const NestedSingleNode& otherNode) const;
    bool operator>=(const NestedSingleNode& otherNode) const;
};

#include "NestedSingleNode.tpp"
#endif
