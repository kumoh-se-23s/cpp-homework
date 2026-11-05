#include "Pair.h"

#include <bits/locale_facets_nonio.h>

template <typename K, typename V>
Pair<K,V>::Pair(const Pair &other)
        : key(other.key),
          value(other.value) {
}
template <typename K, typename V>
Pair<K,V>::Pair(Pair &&other) noexcept {
    key = other.key;
    value = other.value;
}

template <typename K, typename V>
Pair<K,V>& Pair<K,V>::operator=(const Pair &other) {
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
Pair<K,V>& Pair<K,V>::operator=(Pair &&other) noexcept {
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
Pair<K, V>::~Pair() {

}

template<typename K, typename V>
K Pair<K, V>::getKey() const {
}

template<typename K, typename V>
V Pair<K, V>::getValue() const {
}

