#include "DoubleConnectNode.h"


template<typename T>
DoubleConnectNode<T>::DoubleConnectNode()
    : AbstractNode<T, DoubleConnectNode>(), m_next_node(nullptr), m_prev_node(nullptr) {
}

template<typename T>
DoubleConnectNode<T>::DoubleConnectNode(const T &item)
    : AbstractNode<T, DoubleConnectNode>(item), m_next_node(nullptr), m_prev_node(nullptr) {
}

template<typename T>
DoubleConnectNode<T>::DoubleConnectNode(const T &item, DoubleConnectNode *next_node, DoubleConnectNode *prev_node)
    : AbstractNode<T, DoubleConnectNode>(item), m_next_node(next_node), m_prev_node(prev_node) {
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::next() const {
    return this->m_next_node;
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::set_next(DoubleConnectNode *next_node) {
    this->m_next_node = next_node;
    return next_node;
}

template<typename T>
bool DoubleConnectNode<T>::has_next() const {
    return this->m_next_node != nullptr;
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::prev() const {
    return this->m_prev_node;
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::set_prev(DoubleConnectNode *prev_node) {
    this->m_prev_node = prev_node;
    return prev_node;
}

template<typename T>
bool DoubleConnectNode<T>::has_prev() const {
    return this->m_prev_node != nullptr;
}
