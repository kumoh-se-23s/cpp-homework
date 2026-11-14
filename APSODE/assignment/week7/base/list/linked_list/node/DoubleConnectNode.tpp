#include "DoubleConnectNode.h"


template<typename T>
DoubleConnectNode<T>::DoubleConnectNode(const T *item_ptr)
    : AbstractNode<T, DoubleConnectNode>() {
}

template<typename T>
DoubleConnectNode<T>::DoubleConnectNode(const T *item_ptr, DoubleConnectNode<T> *next_node,
                                        DoubleConnectNode<T> *prev_node)
    : AbstractNode<T, DoubleConnectNode>(), m_next_node(next_node), m_prev_node(prev_node) {
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::next() const {
    return this->m_next_node;
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::set_next(const DoubleConnectNode<T> *next_node) {
    this->m_next_node = next_node;
    return next_node;
}

template<typename T>
bool DoubleConnectNode<T>::has_next() {
    return this->m_next_node != nullptr;
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::prev() const {
    return this->m_prev_node;
}

template<typename T>
DoubleConnectNode<T>* DoubleConnectNode<T>::set_prev(const DoubleConnectNode<T> *prev_node) {
    this->m_prev_node = prev_node;
    return prev_node;
}

template<typename T>
bool DoubleConnectNode<T>::has_prev() const {
    return this->m_prev_node != nullptr;
}
