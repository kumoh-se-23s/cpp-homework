#include "KeyList.h"

#include <bits/locale_facets_nonio.h>

template <typename K, typename L>
KeyList<K,L>::KeyList(const KeyList &other)
    : key(other.key),
      list(other.list) {
}
template <typename K, typename L>
KeyList<K,L>::KeyList(KeyList &&other) noexcept {
    key = other.key;
    list= other.list;
    other.key = nullptr;
    other.list = nullptr;
}

template <typename K, typename L>
KeyList<K,L>& KeyList<K,L>::operator=(const KeyList &other) {
    if (this == &other)
        return *this;

    if (key != nullptr) {
        delete key;
    }

    if (list != nullptr) {
        delete list;
    }

    key = other.key;
    list = other.list;
    return *this;
}
template <typename K, typename L>
KeyList<K,L>& KeyList<K,L>::operator=(KeyList &&other) noexcept {
    if (this == &other)
        return *this;

    if (key != nullptr) {
        delete key;
    }

    if (list != nullptr) {
        delete list;
    }

    key = std::move(other.key);
    list = std::move(other.list);

    return *this;
}

template<typename K, typename L>
K KeyList<K,L>::getKey() const {
    return key;
}

template<typename K, typename L>
void KeyList<K, L>::setKey(const K &key) {
    this->key = key;
}

template<typename K, typename L>
L KeyList<K, L>::getList() const {
    return list;
}

template<typename K, typename L>
void KeyList<K, L>::setList(const L &List) {
    this->list = List;
}



