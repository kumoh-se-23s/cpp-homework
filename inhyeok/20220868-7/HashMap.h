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

    static void hash(K key, int capacity) {
        return H::hash(key) % capacity;
    }

    void put(
        GenericCircularLinkedList<KeyValuePair<K, V> > *currTable,
        const int currCapacity,
        KeyValuePair<K, V> &&pair, int &currWeight) {
        const int hashIdx = hash(pair.key, currCapacity);

        bool keyFound = false;
        for (auto itl = currTable[hashIdx].begin(); itl.hasNext(); ++itl) {
            if (itl.hasNext() && (*itl)->data.key == pair.key) {
                keyFound = true;
                break;
            }
        }
        if (!keyFound) {
            V nv;
            currTable[hashIdx].addFirst(RAIISharedPointer(Node(KeyValuePair{
                .key = pair.key,
                .value = std::move(nv)
            })));
            ++currWeight;
        }

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
