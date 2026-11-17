//
// Created by lhetl on 2026-11-17.
//

#ifndef CPP_HOMEWORK_GIT_NESTEDSINGLENODE_H
#define CPP_HOMEWORK_GIT_NESTEDSINGLENODE_H
#include "DoubleNode.h"
#include "SingleNode.h"

template <typename T>
class NestedSingleNode : public SingleNode<DoubleNode<T>> {
public:
    NestedSingleNode(const DoubleNode<T> &item, SingleNode<DoubleNode<T>> *nextNode)
        : SingleNode<DoubleNode<T>>(item, nextNode) {
    }

    explicit NestedSingleNode(const SingleNode<DoubleNode<T>> &other)
        : SingleNode<DoubleNode<T>>(other) {
    }

    explicit NestedSingleNode(SingleNode<DoubleNode<T>> &&other)
        : SingleNode<DoubleNode<T>>(other) {
    }
};


#endif //CPP_HOMEWORK_GIT_NESTEDSINGLENODE_H
