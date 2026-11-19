#pragma once
#include <algorithm>
#include <stdexcept>

#include "RowTable.h"
#include "../util/RequireGuard.h"


template<typename T, RowTraits<T> Traits>
RowTable<T, Traits>::RowTable(int min_key, int max_key)
    : m_buckets(nullptr), m_base_key(min_key), m_bucket_count(max_key - min_key + 1) {
    if (min_key > max_key) {
        throw std::invalid_argument("Invalid key range: expected min_key <= max_key.");
    }

    this->m_buckets = new RowChain<T, Traits>[this->m_bucket_count];
}

template<typename T, RowTraits<T> Traits>
RowTable<T, Traits>::~RowTable() {
    delete[] this->m_buckets;
}

template<typename T, RowTraits<T> Traits>
void RowTable<T, Traits>::link(MultiConnectNode<T> *node) {
    require_non_null(node);

    int row_key = Traits::row_key(node->get());

    if (!this->in_range(row_key)) {
        this->grow_to(row_key);
    }

    this->bucket_of(row_key).insert(node);
}

template<typename T, RowTraits<T> Traits>
void RowTable<T, Traits>::unlink(MultiConnectNode<T> *node) {
    require_non_null(node);

    int row_key = Traits::row_key(node->get());

    if (!this->in_range(row_key)) {
        throw std::invalid_argument("Node is not linked: row key is out of range.");
    }

    this->bucket_of(row_key).remove(node);
}

template<typename T, RowTraits<T> Traits>
MultiConnectNode<T>* RowTable<T, Traits>::find_if(int row_key, std::function<bool(const MultiConnectNode<T> *node)> pred) const {
    if (!this->in_range(row_key)) {
        return nullptr;
    }

    return this->bucket_of(row_key).find_if(pred);
}

template<typename T, RowTraits<T> Traits>
int RowTable<T, Traits>::min_key() const {
    for (int index = 0; index < this->m_bucket_count; ++index) {
        if (!this->m_buckets[index].empty()) {
            return this->m_base_key + index;
        }
    }

    return this->m_base_key;
}

template<typename T, RowTraits<T> Traits>
int RowTable<T, Traits>::max_key() const {
    for (int index = this->m_bucket_count - 1; index >= 0; --index) {
        if (!this->m_buckets[index].empty()) {
            return this->m_base_key + index;
        }
    }

    return this->last_key();
}

template<typename T, RowTraits<T> Traits>
void RowTable<T, Traits>::for_each(std::function<void(MultiConnectNode<T> *node)> visit) const {
    for (int index = 0; index < this->m_bucket_count; ++index) {
        this->m_buckets[index].for_each(visit);
    }
}

template<typename T, RowTraits<T> Traits>
bool RowTable<T, Traits>::in_range(int row_key) const {
    return this->m_base_key <= row_key && row_key <= this->last_key();
}

template<typename T, RowTraits<T> Traits>
RowChain<T, Traits>& RowTable<T, Traits>::bucket_of(int row_key) const {
    return this->m_buckets[row_key - this->m_base_key];
}

template<typename T, RowTraits<T> Traits>
int RowTable<T, Traits>::last_key() const {
    return this->m_base_key + this->m_bucket_count - 1;
}

template<typename T, RowTraits<T> Traits>
void RowTable<T, Traits>::grow_to(int row_key) {
    int new_base_key = std::min(row_key, this->m_base_key);
    int new_bucket_count = std::max(row_key, this->last_key()) - new_base_key + 1;

    RowChain<T, Traits> *new_buckets = new RowChain<T, Traits>[new_bucket_count];

    for (int key = this->m_base_key; key <= this->last_key(); ++key) {
        new_buckets[key - new_base_key].take(this->bucket_of(key));
    }

    delete[] this->m_buckets;

    this->m_buckets = new_buckets;
    this->m_base_key = new_base_key;
    this->m_bucket_count = new_bucket_count;
}
