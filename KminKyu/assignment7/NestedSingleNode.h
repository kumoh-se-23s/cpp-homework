#ifndef NESTEDSINGLENODE_H
#define NESTEDSINGLENODE_H
#include "SingleNode.h"
#include "Node.h"

template <typename T>
class NestedSingleNode : public SingleNode<Node<T>> {
public:
    ~NestedSingleNode() = default;

    NestedSingleNode(const Node<T> &item, SingleNode<Node<T>> *nextNode)
        : SingleNode<Node<T>>(item, nextNode) {
    }

    NestedSingleNode(const SingleNode<Node<T>> &other)
        : SingleNode<Node<T>>(other) {
    }

    NestedSingleNode(SingleNode<Node<T>> &&other)
        : SingleNode<Node<T>>(other) {
    }

    const T& getUnravelItem();
};



#include "NestedSingleNode.tpp"
#endif
