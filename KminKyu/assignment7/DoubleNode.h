#ifndef DOUBLENODE_H
#define DOUBLENODE_H
#include <utility>

template <typename T>
class DoubleNode {
    T item;
    DoubleNode<T> *prevNode = nullptr;
    DoubleNode<T> *nextNode = nullptr;
public:
    DoubleNode(const T &item, DoubleNode<T> *prevNode, DoubleNode<T> *nextNode)
        : item(item),
          prevNode(prevNode),
          nextNode(nextNode) {
    }

    DoubleNode(const DoubleNode &other)
        : item(other.item),
          prevNode(other.prevNode),
          nextNode(other.nextNode) {
    }

    DoubleNode(DoubleNode &&other) noexcept
        : item(std::move(other.item)),
          prevNode(other.prevNode),
          nextNode(other.nextNode) {
    }

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
