#pragma once
#include "MultiConnectNode.h"


template<typename T>
MultiConnectNode<T>::MultiConnectNode()
    : AbstractNode<T, MultiConnectNode>() {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const T &item)
    : AbstractNode<T, MultiConnectNode>(item) {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const MultiConnectNode &row_node, const MultiConnectNode &column_node)
    : AbstractNode<T, MultiConnectNode>(),
      m_row_next(row_node.m_row_next), m_row_prev(row_node.m_row_prev),
      m_column_next(column_node.m_column_next), m_column_prev(column_node.m_column_prev) {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const T &item, const MultiConnectNode &row_node, const MultiConnectNode &column_node)
    : AbstractNode<T, MultiConnectNode>(item),
      m_row_next(row_node.m_row_next), m_row_prev(row_node.m_row_prev),
      m_column_next(column_node.m_column_next), m_column_prev(column_node.m_column_prev) {
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::row_next() const {
    return this->m_row_next;
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::set_row_next(MultiConnectNode *next_node) {
    this->m_row_next = next_node;
    return next_node;
}

template<typename T>
bool MultiConnectNode<T>::has_row_next() const {
    return this->m_row_next != nullptr;
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::row_prev() const {
    return this->m_row_prev;
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::set_row_prev(MultiConnectNode *prev_node) {
    this->m_row_prev = prev_node;
    return prev_node;
}

template<typename T>
bool MultiConnectNode<T>::has_row_prev() const {
    return this->m_row_prev != nullptr;
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::column_next() const {
    return this->m_column_next;
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::set_column_next(MultiConnectNode *next_node) {
    this->m_column_next = next_node;
    return next_node;
}

template<typename T>
bool MultiConnectNode<T>::has_column_next() const {
    return this->m_column_next != nullptr;
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::column_prev() const {
    return this->m_column_prev;
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::set_column_prev(MultiConnectNode *prev_node) {
    this->m_column_prev = prev_node;
    return prev_node;
}

template<typename T>
bool MultiConnectNode<T>::has_column_prev() const {
    return this->m_column_prev != nullptr;
}
