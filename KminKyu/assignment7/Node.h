#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
protected:
    T item;
    Node(const T &item) : item(item) {}
    Node(const Node &) = default;
    Node(Node &&) noexcept = default;
public:
    virtual ~Node() = default;
    virtual const T& getItem() const = 0;
};
#endif // NODE_H
