#pragma once
#include "SingleConnectNode.h"


template<typename T>
SingleConnectNode<T>::SingleConnectNode()
    : AbstractNode<T, SingleConnectNode>(), m_next_node(nullptr) {
}

template<typename T>
SingleConnectNode<T>::SingleConnectNode(const T &item)
    : AbstractNode<T, SingleConnectNode>(item), m_next_node(nullptr) {
}

template<typename T>
SingleConnectNode<T>::SingleConnectNode(const T &item, SingleConnectNode *next_node)
    : AbstractNode<T, SingleConnectNode>(item), m_next_node(next_node) {
}


template<typename T>
SingleConnectNode<T>* SingleConnectNode<T>::next() const {
    return this->m_next_node;
}

template<typename T>
SingleConnectNode<T>* SingleConnectNode<T>::set_next(SingleConnectNode *next_node) {
    this->m_next_node = next_node;
    return next_node;
}

template<typename T>
bool SingleConnectNode<T>::has_next() const {
    return this->m_next_node != nullptr;
}
