#pragma once
#include <stdexcept>

#include "RowLinkPolicy.h"


template<typename T>
MultiConnectNode<T>* RowLinkPolicy<T>::do_next(const MultiConnectNode<T> *node) {
    return node->row_next();
}

template<typename T>
void RowLinkPolicy<T>::do_link_front(MultiConnectNode<T> *&head, MultiConnectNode<T> *node) {
    node->set_row_next(head);
    head = node;
}

template<typename T>
void RowLinkPolicy<T>::do_link_after(MultiConnectNode<T> *position, MultiConnectNode<T> *node) {
    node->set_row_next(position->row_next());
    position->set_row_next(node);
}

template<typename T>
void RowLinkPolicy<T>::do_unlink(MultiConnectNode<T> *&head, MultiConnectNode<T> *node) {
    if (head == node) {
        head = node->row_next();
        RowLinkPolicy::do_reset(node);
        return;
    }

    MultiConnectNode<T> *prev_node = head;

    for (; prev_node != nullptr && prev_node->row_next() != node; prev_node = prev_node->row_next()) {
    }

    if (prev_node == nullptr) {
        throw std::invalid_argument("Node is not linked: expected a node contained in this chain.");
    }

    prev_node->set_row_next(node->row_next());
    RowLinkPolicy::do_reset(node);
}

template<typename T>
void RowLinkPolicy<T>::do_reset(MultiConnectNode<T> *node) {
    node->set_row_next(nullptr);
}
