#ifndef SINGLENODE_H
#define SINGLENODE_H

#include <iostream>
#include <utility>

#include "OOldMyString.h"
#include "OldNode.h"


template <typename T>
class OldSingleNode : public OldNode<T>{

    OldSingleNode *nextNode = nullptr;
public:
    OldSingleNode() = default;

    OldSingleNode(const T &item, OldSingleNode *nextNode);

    OldSingleNode(const T &item);

    OldSingleNode(const OldSingleNode &other) = delete;

    OldSingleNode(OldSingleNode &&other) noexcept;

    ~OldSingleNode() override = default;

    OldSingleNode &operator=(const OldSingleNode &other) = delete;

    OldSingleNode &operator=(OldSingleNode &&other) noexcept;

    OldSingleNode *getNext() const;

    void setNext(OldSingleNode *next);

};

#include "OldSingleNode.tpp"
#endif
