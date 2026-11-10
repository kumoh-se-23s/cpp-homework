//
// Created by Merutilm on 10/30/26.
//

#pragma once

#include "GenericCircularLinkedList.h"
#include "KeyValuePair.h"

template<typename K, typename V, typename H>
class HashMap {
    GenericCircularLinkedList<KeyValuePair<K, V> > *table = nullptr;
    int weight = 0;
    int capacity = 0;
    constexpr static int INITIAL_CAPACITY = 16;

public:
    explicit HashMap(const int initialCapacity = INITIAL_CAPACITY) {
        table = new GenericCircularLinkedList<KeyValuePair<K, V> >[initialCapacity];
        capacity = initialCapacity;
    }

    ~HashMap() {
        delete[] table;
    }

    [[nodiscard]] bool containsKey(const K &key) {
        int idx = hash(key, capacity);
        for (auto it = table[idx].begin(); it.hasNext(); ++it) {
            if ((*it)->data.key == key) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] V *operator[](const K &key) {
        int idx = hash(key, capacity);
        for (auto it = table[idx].begin(); it.hasNext(); ++it) {
            if ((*it)->data.key == key) {
                return &(*it)->data.value;
            }
        }
        return nullptr;
    }

    void put(K key, V value) {
        if (capacity == 0) {
            resize(INITIAL_CAPACITY);
        }

        KeyValuePair pair{
            .key = key,
            .value = value
        };
        put(table, capacity, std::move(pair), weight);
    }


    void remove(K key) {
        const int hashIdx = hash(key, capacity);

        for (auto it = table[hashIdx].begin(); it.hasNext(); ++it) {
            KeyValuePair<K, V> &entry = (*it)->data;
            if (entry.key == key) {
                table[hashIdx].remove(*it);
            }
        }
    }

private:
    static size_t hash(K key, int capacity) {
        return H::hash(key) % capacity;
    }

    void put(
        GenericCircularLinkedList<KeyValuePair<K, V> > *currTable,
        const int currCapacity,
        KeyValuePair<K, V> &&pair, int &currWeight) {
        const int hashIdx = hash(pair.key, currCapacity);

        for (auto itl = currTable[hashIdx].begin(); itl.hasNext(); ++itl) {
            if (itl.hasNext() && (*itl)->data.key == pair.key) {
                (*itl)->data = std::move(pair);
                return;
            }
        }
        currTable[hashIdx].addFirst(RAIISharedPointer(DoubleNode(std::move(pair))));
        ++currWeight;
    }

    void resize(int newCapacity) {
        const auto newTable = new GenericCircularLinkedList<KeyValuePair<K, V> >[newCapacity];
        weight = 0;
        for (int i = 0; i < capacity; ++i) {
            for (auto it = table[i].begin(); it.hasNext(); ++it) {
                put(newTable, newCapacity, std::move((*it)->data), weight);
            }
        }

        delete[] table;
        table = newTable;
        capacity = newCapacity;
    }
};
