#pragma once
#include "LinkedList.h"


template <typename K, typename V>
class KeyList {
    K key;
    LinkedList<V> linkedList;
public:
    KeyList() = default;

    KeyList(const K &key, LinkedList<V> linkedList): key(key),linkedList(linkedList) {}

    ~KeyList();

    KeyList(const KeyList &other);

    KeyList(KeyList &&other) noexcept;

    KeyList & operator=(const KeyList &other);

    KeyList & operator=(KeyList &&other) noexcept;

    K getKey() const;

    LinkedList<V> getLinkedList() const;

    void setKey(const K &key);

};



