#ifndef ENTRY_H
#define ENTRY_H

#include  <iostream>
#include "Concept.h"
#include "MyString.h"

template <typename K, typename V>
class Entry {
    K key;
    V *value;
public:
    Entry() : key(), value(nullptr) {}

    Entry(const K &k, V *v) : key(k), value(v) {}

    ~Entry();

    Entry(const Entry &) = delete;

    Entry &operator=(const Entry &) = delete;

    Entry(Entry &&other) noexcept;

    Entry &operator=(Entry &&other) noexcept;

    MyString toString() const requires HasToString<V>;

    const K& getKey() const;

    V *getValue() const;
};


template <typename K, typename V>
std::ostream &operator<<(std::ostream &out, const Entry<K, V>& entry);


#include "Entry.tpp"
#endif // ENTRY_H
