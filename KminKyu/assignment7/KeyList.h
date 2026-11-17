#pragma once
#include "LinkedList.h"


template <typename K, typename L>
class KeyList {
    K key;
    L list;
public:
    KeyList() = default;

    KeyList(const K &key, L &list): key(key), list(list) {}

    ~KeyList() = default;

    KeyList(const KeyList &other);

    KeyList(KeyList &&other) noexcept;

    KeyList & operator=(const KeyList &other);

    KeyList & operator=(KeyList &&other) noexcept;

    K getKey() const;

    void setKey(const K &key);

    L getList() const;

    void setList(const L &List);

};



