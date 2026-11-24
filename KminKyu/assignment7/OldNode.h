#ifndef NODE_H
#define NODE_H

#include <utility>
#include <iostream>

#include "Concept.h"
#include "OOldMyString.h"

template <typename T>
class OldNode {
protected:
    T item;

    OldNode(const T &item) : item(item) {}

    OldNode(const OldNode &other) : item(other.item) {}

    OldNode(OldNode &&other) noexcept : item(std::move(other.item)) {}

    OldNode &operator=(const OldNode &other);

    OldNode &operator=(OldNode &&other) noexcept;

public:

    virtual ~OldNode() = default;

    const T& getItem() const;

    T& getItem();

    void setItem(const T &item);

    OOldMyString toString() const requires HasToString<T>;
};
template <typename T>
std::ostream& operator<<(std::ostream& out, const OldNode<T>& node);


#include "OldNode.tpp"
#endif // NODE_H
