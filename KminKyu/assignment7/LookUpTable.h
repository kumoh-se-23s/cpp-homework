#ifndef KEYLIST_H
#define KEYLIST_H
#include "LinkedList.h"


template <typename K, typename V>
struct Entry {
    K key;
    V value;
};
template <typename K, typename V>
class LookUpTable {

    Entry<K, V> *entryArray;

    int size = 0;

    const static int DEFAULT_CAPACITY = 4;

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

    Entry<K, V> getEntry(K key) const;

    void setList(const LinkedList<V> &List);

};


#include "LookUpTable.tpp"

#endif // KEYLIST_H
