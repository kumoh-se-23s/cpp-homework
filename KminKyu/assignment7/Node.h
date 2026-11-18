#ifndef NODE_H
#define NODE_H
#include <algorithm>

template <typename T>
class Node {
protected:
    T item;

public:
    Node(const T &item) : item(item) {}

    Node(const Node &other) : item(other.item) {}

    Node(Node &&other) noexcept : item(std::move(other.item)) {}

    virtual ~Node() = default;

    const T& getItem() const;

    T& getItem();

    void setItem(T &item);
};

#include "Node.tpp"
#endif // NODE_H
