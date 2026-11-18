#ifndef KEYLIST_H
#define KEYLIST_H

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

    Entry<K, V>& getEntry(K key) const;

    const V& getValue(K Key) const;

    void putIfAbsent(K key, V value) ;

    void insert(int index, K key, V value);
private:
    int binarySearch(int left, int right, K key) const;
};

#include "LookUpTable.tpp"

#endif // KEYLIST_H
