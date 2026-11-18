#ifndef KEYLIST_H
#define KEYLIST_H
#include "LinkedList.h"


template <typename K, typename L>
struct Entry {
    K key;
    L list;
};

template <typename K, typename V>
class KeyArray {
    Entry<K, LinkedList<V>>* entryArray;

    int size = 0;

    const static int DEFAULT_CAPACITY = 4;

    int capacity = DEFAULT_CAPACITY;

public:
    KeyArray();

    KeyArray(int capacity) : capacity(capacity), entryArray(new Entry<K, LinkedList<V>>()){}

    ~KeyArray() = default;

    KeyArray(const KeyArray &other);

    KeyArray(KeyArray &&other) noexcept;

    KeyArray & operator=(const KeyArray &other);

    KeyArray & operator=(KeyArray &&other) noexcept;

    void putIfAbsent(K key, V value);

    void put(K key, V value);

    void append(K key, V value);

    void resize();

    L getList(K key) const;

    void setList(const LinkedList<V> &List);

};


#include "KeyArray.tpp"

#endif // KEYLIST_H
