#pragma once
#include "MultiConnectNode.h"


template<typename T>
MultiConnectNode<T>::MultiConnectNode()
    : AbstractNode<T, MultiConnectNode>(),
      m_row_next(nullptr), m_column_next(nullptr), m_column_prev(nullptr) {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const T &item)
    : AbstractNode<T, MultiConnectNode>(item),
      m_row_next(nullptr), m_column_next(nullptr), m_column_prev(nullptr) {
}

template<typename T>
MultiConnectNode<T>* MultiConnectNode<T>::row_next() const {
    return this->m_row_next;
}

template<typename T>
void MultiConnectNode<T>::set_row_next(MultiConnectNode *node) {
    this->m_row_next = node;
}

template<typename T>
MultiConnectNode<T>* MultiConnectNode<T>::column_next() const {
    return this->m_column_next;
}

template<typename T>
void MultiConnectNode<T>::set_column_next(MultiConnectNode *node) {
    this->m_column_next = node;
}

template<typename T>
MultiConnectNode<T>* MultiConnectNode<T>::column_prev() const {
    return this->m_column_prev;
}

template<typename T>
void MultiConnectNode<T>::set_column_prev(MultiConnectNode *node) {
    this->m_column_prev = node;
}
