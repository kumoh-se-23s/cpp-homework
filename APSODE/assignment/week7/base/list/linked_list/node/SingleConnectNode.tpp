#include "SingleConnectNode.h"


template<typename T>
SingleConnectNode<T>::SingleConnectNode(const T *item)
    : AbstractNode<T, SingleConnectNode>(item), m_next_node(nullptr) {
}

template<typename T>
SingleConnectNode<T>::SingleConnectNode(const T *item, const SingleConnectNode<T> *next_node)
    : AbstractNode<T, SingleConnectNode>(item), m_next_node(next_node) {
}


template<typename T>
SingleConnectNode<T>* SingleConnectNode<T>::next() const {
    return this->m_next_node;
}

template<typename T>
SingleConnectNode<T>* SingleConnectNode<T>::set_next(const SingleConnectNode<T> *next_node) {
    this->m_next_node = next_node;
    return next_node;
}

template<typename T>
bool SingleConnectNode<T>::has_next() {
    return this->m_next_node != nullptr;
}
