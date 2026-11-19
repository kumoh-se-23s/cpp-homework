#pragma once
#include <stdexcept>

#include "ColumnTable.h"
#include "../util/RequireGuard.h"


template<typename T, ColumnTraits<T> Traits>
ColumnTable<T, Traits>::Entry::Entry(const MyString &key)
    : key(key), ring(), next(nullptr) {
}

template<typename T, ColumnTraits<T> Traits>
ColumnTable<T, Traits>::ColumnTable()
    : m_buckets(nullptr), m_capacity(DEFAULT_CAPACITY), m_entry_count(0) {
    this->m_buckets = new Entry*[this->m_capacity]();
}

template<typename T, ColumnTraits<T> Traits>
ColumnTable<T, Traits>::~ColumnTable() {
    for (int index = 0; index < this->m_capacity; ++index) {
        for (Entry *current_entry = this->m_buckets[index], *next_entry = nullptr; current_entry != nullptr; current_entry = next_entry) {
            next_entry = current_entry->next;
            delete current_entry;
        }
    }

    delete[] this->m_buckets;
}

template<typename T, ColumnTraits<T> Traits>
void ColumnTable<T, Traits>::link(MultiConnectNode<T> *node) {
    require_non_null(node);

    if (!Traits::has_column(node->get())) {
        return;
    }

    Entry *entry = this->find_or_create_entry(Traits::column_key(node->get()));
    entry->ring.insert(node);
}

template<typename T, ColumnTraits<T> Traits>
void ColumnTable<T, Traits>::unlink(MultiConnectNode<T> *node) {
    require_non_null(node);

    if (!Traits::has_column(node->get())) {
        return;
    }

    MyString key = Traits::column_key(node->get());
    Entry *entry = this->find_entry(key);

    if (entry == nullptr) {
        throw std::invalid_argument("Node is not linked: column key does not exist.");
    }

    entry->ring.remove(node);

    if (entry->ring.empty()) {
        this->remove_entry(key);
    }
}

template<typename T, ColumnTraits<T> Traits>
void ColumnTable<T, Traits>::for_each(const MultiConnectNode<T> *node, std::function<void(const MultiConnectNode<T> *mate)> visit) const {
    require_non_null(node);

    if (!Traits::has_column(node->get())) {
        return;
    }

    Entry *entry = this->find_entry(Traits::column_key(node->get()));

    if (entry == nullptr) {
        return;
    }

    entry->ring.for_each(node, visit);
}

template<typename T, ColumnTraits<T> Traits>
int ColumnTable<T, Traits>::count() const {
    return this->m_entry_count;
}

template<typename T, ColumnTraits<T> Traits>
int ColumnTable<T, Traits>::hash(const MyString &key) const {
    unsigned int hash_value = 0;

    for (int index = 0; index < key.length(); ++index) {
        hash_value = hash_value * 31 + static_cast<unsigned char>(key[index]);
    }

    return static_cast<int>(hash_value % this->m_capacity);
}

template<typename T, ColumnTraits<T> Traits>
ColumnTable<T, Traits>::Entry* ColumnTable<T, Traits>::find_entry(const MyString &key) const {
    for (Entry *current_entry = this->m_buckets[this->hash(key)]; current_entry != nullptr; current_entry = current_entry->next) {
        if (current_entry->key == key) {
            return current_entry;
        }
    }

    return nullptr;
}

template<typename T, ColumnTraits<T> Traits>
ColumnTable<T, Traits>::Entry* ColumnTable<T, Traits>::find_or_create_entry(const MyString &key) {
    Entry *entry = this->find_entry(key);

    if (entry != nullptr) {
        return entry;
    }

    int index = this->hash(key);

    entry = new Entry(key);

    entry->next = this->m_buckets[index];
    this->m_buckets[index] = entry;
    ++this->m_entry_count;

    return entry;
}

template<typename T, ColumnTraits<T> Traits>
void ColumnTable<T, Traits>::remove_entry(const MyString &key) {
    int index = this->hash(key);
    Entry *prev_entry = nullptr;
    Entry *current_entry = this->m_buckets[index];

    for (; current_entry != nullptr && current_entry->key != key; current_entry = current_entry->next) {
        prev_entry = current_entry;
    }

    if (current_entry == nullptr) {
        return;
    }

    if (prev_entry == nullptr) {
        this->m_buckets[index] = current_entry->next;
    } else {
        prev_entry->next = current_entry->next;
    }

    delete current_entry;
    --this->m_entry_count;
}
