#pragma once

template<typename K, typename V>
class Entry;

template<typename K, typename V>
Entry<K, V>::Entry(Entry &&other) noexcept {
    key = std::move(other.key);
    value = other.value;
    other.value = nullptr;
}
template<typename K, typename V>
Entry<K, V>::~Entry() {
    delete value;
}

template<typename K, typename V>
Entry<K, V> & Entry<K, V>::operator=(Entry &&other) noexcept {
    if (this != &other) {
        delete value;
        key = std::move(other.key);
        value = other.value;
        other.value = nullptr;
    }
    return *this;
}

template<typename K, typename V>
const K& Entry<K, V>::getKey() const {
    return key;
}

template<typename K, typename V>
V *Entry<K, V>::getValue() const {
    return value;
}

template<typename K, typename V>
MyString Entry<K, V>::toString() const requires HasToString<V> {
    MyString result = MyString();
    if (value != nullptr) {
        result += value->toString();
    }
    return result;
}

template<typename K, typename V>
std::ostream & operator<<(std::ostream &out, const Entry<K, V> &entry) {
    if (entry.getValue() != nullptr) {
        out << *entry.getValue();
    }
    return out;
}


