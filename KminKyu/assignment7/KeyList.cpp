#include "KeyList.h"

#include <bits/locale_facets_nonio.h>

template <typename K, typename V>
KeyList<K,V>::KeyList(const KeyList &other)
        : key(other.key),
          value(other.value) {
}
template <typename K, typename V>
KeyList<K,V>::KeyList(KeyList &&other) noexcept {
    key = other.key;
    value = other.value;
}

template <typename K, typename V>
KeyList<K,V>& KeyList<K,V>::operator=(const KeyList &other) {
    if (this == &other)
        return *this;

    if (key != nullptr) {
        delete key;
    }

    if (value != nullptr) {
        delete value;
    }

    key = other.key;
    value = other.value;
    return *this;
}
template <typename K, typename V>
KeyList<K,V>& KeyList<K,V>::operator=(KeyList &&other) noexcept {
    if (this == &other)
        return *this;

    if (key != nullptr) {
        delete key;
    }

    if (value != nullptr) {
        delete value;
    }

    key = std::move(other.key);
    value = std::move(other.value);

    return *this;
}

template<typename K, typename V>
V KeyList<K, V>::getValue() const {
    return value;
}

template<typename K, typename V>
K KeyList<K,V>::getKey() const {
    return key;
}

template<typename K, typename V>
LinkedList<V> KeyList<K, V>::getLinkedList() const {
    return linkedList;
}

template<typename K, typename V>
void KeyList<K, V>::setKey(const K &key) {
    this->key = key;
}

template<typename K, typename V>
void KeyList<K, V>::setValue(const V &value) {
    this->value = value;
}



