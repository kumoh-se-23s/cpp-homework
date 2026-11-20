#ifndef LOOKUPTABLE_H
#define LOOKUPTABLE_H

#include <iostream>
#include <utility>

#include "Entry.h"
#include "MyString.h"

template <typename K, typename V>
class LookUpTable {

    Entry<K, V> *entryArray;

    int size = 0;

    static constexpr int DEFAULT_CAPACITY = 4;

    int capacity = DEFAULT_CAPACITY;

public:

    LookUpTable();

    LookUpTable(int capacity);

    LookUpTable(const LookUpTable &other) = delete;

    LookUpTable(LookUpTable &&other) noexcept;

    ~LookUpTable();

    LookUpTable & operator=(const LookUpTable &other) = delete;

    LookUpTable & operator=(LookUpTable &&other) noexcept;

    void resize();

    const Entry<K, V> *at(int index) const;

    Entry<K, V> *findEntry(const K &key);

    const Entry<K, V> *findEntry(const K &key) const;

    V *getValue(const K &key) const;

    void removeByKey(const K &key);

    void insert(const K& key, V* value);

    int getSize() const;

    int getCapacity() const;

    MyString toString() const;

    bool isEntry(const K& key);

private:
    int binarySearch(int left, int right, const K& key) const;

    void insert(int index, const K& key, V* value);
};

template <typename K, typename V>
std::ostream &operator<<(std::ostream &out, const LookUpTable<K, V>& lookUpTable);



#include "LookUpTable.tpp"

#endif // LOOKUPTABLE_H
