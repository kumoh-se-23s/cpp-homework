#ifndef NODE_H
#define NODE_H

#include <algorithm>
#include <utility>

#include "MyString.h"

template <typename T>
concept HasToString = requires(const T &t) {
    { t.toString() } -> std::convertible_to<MyString>;
};

template <typename T>
class Node {
protected:
    T item;

    Node(const T &item) : item(item) {}

    Node(const Node &other) : item(other.item) {}

    Node(Node &&other) noexcept : item(std::move(other.item)) {}

    Node &operator=(const Node &other);

    Node &operator=(Node &&other) noexcept;

public:

    virtual ~Node() = default;

    const T& getItem() const;

    T& getItem();

    void setItem(const T &item);

    MyString toString() const requires HasToString<T>;
};

#include "Node.tpp"
#endif // NODE_H
