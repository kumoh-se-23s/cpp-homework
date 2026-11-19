#ifndef LOOKUPTABLE_H
#define LOOKUPTABLE_H
#include "MyString.h"

template <typename K, typename V>
struct Entry {
    K key;
    V value;
};
template <typename K, typename V>
class LookUpTable {

    Entry<K, V> *entryArray;

    int size = 0;

    static constexpr int DEFAULT_CAPACITY = 4;

    int capacity = DEFAULT_CAPACITY;

public:

    LookUpTable();

    LookUpTable(int capacity);

    LookUpTable(const LookUpTable &other);

    LookUpTable(LookUpTable &&other) noexcept;

    ~LookUpTable();

    LookUpTable & operator=(const LookUpTable &other);

    LookUpTable & operator=(LookUpTable &&other) noexcept;

    void resize();

    Entry<K, V> *findEntry(const K &key);

    const Entry<K, V> *findEntry(const K &key) const;

    const V &getValue(const K &key) const;

    const V &getByIndex(int index) const;

    void removeByKey(const K &key) const;

    void setValue(const K& key, const V& value);

    const V &insert(const K& key, const V& value);

    int getSize() const;

    int getCapacity() const;

private:
    int binarySearch(int left, int right, const K& key) const;

    const V &insert(int index, const K& key, const V& value);

};

#include "LookUpTable.tpp"

#endif // LOOKUPTABLE_H
