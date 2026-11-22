#ifndef NODE_H
#define NODE_H

#include <utility>
#include <iostream>

template <typename T>
class Node {
protected:
    T *item;

    Node(const T *item) : item(item) {}

    Node(const Node &other) : item(other.item) {}

    Node(Node &&other) noexcept : item(std::move(other.item)) {}

    Node &operator=(const Node &other);

    Node &operator=(Node &&other) noexcept;

public:

    virtual ~Node() = default;

    const T& getItem() const;

    T& getItem();

    void setItem(const T &item);

};
template <typename T>
std::ostream& operator<<(std::ostream& out, const Node<T>& node);

template <typename T>
std::ofstream& operator<<(std::ofstream& out,  const Node<T>& node);
#include "Node.tpp"
#endif // NODE_H
