#pragma once

#include "MultiLinkedList.h"


template<typename T, MultiTraits<T> Traits>
MultiLinkedList<T, Traits>::MultiLinkedList(int min_row_key, int max_row_key)
    : m_rows(min_row_key, max_row_key), m_columns(), m_size(0) {
}

template<typename T, MultiTraits<T> Traits>
MultiLinkedList<T, Traits>::~MultiLinkedList() {
    this->m_rows.for_each([](MultiConnectNode<T> *node) {
        MultiLinkedList::destroy_node(node);
    });
}

template<typename T, MultiTraits<T> Traits>
bool MultiLinkedList<T, Traits>::insert(const T &item) {
    if (this->find_node(item) != nullptr) {
        return false;
    }

    MultiConnectNode<T> *node = MultiLinkedList::create_node(item);

    this->m_rows.link(node);
    this->m_columns.link(node);
    ++this->m_size;

    return true;
}

template<typename T, MultiTraits<T> Traits>
bool MultiLinkedList<T, Traits>::remove(const T &probe) {
    MultiConnectNode<T> *node = this->find_node(probe);

    if (node == nullptr) {
        return false;
    }

    this->m_columns.unlink(node);
    this->m_rows.unlink(node);
    MultiLinkedList::destroy_node(node);
    --this->m_size;

    return true;
}

template<typename T, MultiTraits<T> Traits>
const T* MultiLinkedList<T, Traits>::find(const T &probe) const {
    const MultiConnectNode<T> *node = this->find_node(probe);

    if (node == nullptr) {
        return nullptr;
    }

    return &node->get();
}

template<typename T, MultiTraits<T> Traits>
int MultiLinkedList<T, Traits>::size() const {
    return this->m_size;
}

template<typename T, MultiTraits<T> Traits>
bool MultiLinkedList<T, Traits>::empty() const {
    return this->m_size == 0;
}

template<typename T, MultiTraits<T> Traits>
int MultiLinkedList<T, Traits>::column_count() const {
    return this->m_columns.count();
}

template<typename T, MultiTraits<T> Traits>
int MultiLinkedList<T, Traits>::min_row_key() const {
    return this->m_rows.min_key();
}

template<typename T, MultiTraits<T> Traits>
int MultiLinkedList<T, Traits>::max_row_key() const {
    return this->m_rows.max_key();
}

template<typename T, MultiTraits<T> Traits>
void MultiLinkedList<T, Traits>::for_each(std::function<void(const T &item)> visit) const {
    this->m_rows.for_each([&visit](const MultiConnectNode<T> *node) {
        visit(node->get());
    });
}

template<typename T, MultiTraits<T> Traits>
void MultiLinkedList<T, Traits>::for_each(const T &probe, std::function<void(const T &mate)> visit) const {
    const MultiConnectNode<T> *node = this->find_node(probe);

    if (node == nullptr) {
        return;
    }

    this->m_columns.for_each(node, [&visit](const MultiConnectNode<T> *mate) {
        visit(mate->get());
    });
}

template<typename T, MultiTraits<T> Traits>
MultiConnectNode<T>* MultiLinkedList<T, Traits>::find_node(const T &probe) const {
    return this->m_rows.find_if(Traits::row_key(probe), [&probe](const MultiConnectNode<T> *node) {
        return Traits::same(node->get(), probe);
    });
}

template<typename T, MultiTraits<T> Traits>
MultiConnectNode<T>* MultiLinkedList<T, Traits>::create_node(const T &item) {
    return new MultiConnectNode<T>(item);
}

template<typename T, MultiTraits<T> Traits>
void MultiLinkedList<T, Traits>::destroy_node(MultiConnectNode<T> *node) {
    delete node;
}
