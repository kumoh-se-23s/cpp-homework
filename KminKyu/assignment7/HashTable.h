#pragma once
#include<iostream>
#include "Pair.h"

template <typename K, typename V>
class HashTable {
    const static int DEFAULT_CAPACITY = 8;
    Pair<K, V> *tables = nullptr;
    int size = 0;
    int capacity = 0;
public:
    HashTable(int c = DEFAULT_CAPACITY) : Pair(new Pair[std::max(c, 0)]), capacity(std::max(c, 0)), size(0) {}

    HashTable(const HashTable &otherArray);

    HashTable(HashTable &&otherArray) noexcept;

    HashTable(const Pair<K,V> *objectArray);

    HashTable(Pair<K,V> *&&objectArray);

    ~HashTable();

    HashTable operator =(const HashTable &otherArray);

    HashTable operator =(HashTable &&otherSet) noexcept;

    HashTable operator +(const HashTable &otherSet) const;

    HashTable operator +(const Pair<K,V> *objectArray) const;

    HashTable operator +(const Pair<K,V> &object) const;

    int getLength() const;

    int getCapacity() const;

    int get(int index) const;

    void clear();

    void resize();
private:
    void append(const Pair<K,V> &appendValue);

    void append(const Pair<K,V> *appendValue);

    void append(const HashTable &objectArray);
};

template <typename K, typename V>
std::istream& operator>>(std::istream& in, HashTable<K,V>& arrayList);

template <typename K, typename V>
std::ostream& operator<<(std::ostream& out, const HashTable<K,V>& arrayList);




