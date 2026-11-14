#pragma once
#include <algorithm>

template <typename T>
class SingleNode {
    T item;
    SingleNode<T> *nextNode = nullptr;
public:
    SingleNode(const T &item, SingleNode<T> *nextNode)
        : item(item),
          nextNode(nextNode) {
    }

    SingleNode(const SingleNode &other)
        : item(other.item),
          nextNode(other.nextNode) {
    }

    SingleNode(SingleNode &&other) noexcept
        : item(std::move(other.item)),
          nextNode(other.nextNode) {
    }

    SingleNode<T> &operator=(const SingleNode &other);

    SingleNode<T> &operator=(SingleNode &&other) noexcept;

    ~SingleNode() = default;

    T getItem() const;

    void setItem(const T &item);


};

#include "SingleNode.tpp"