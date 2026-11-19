#pragma once
#include "ColumnLinkPolicy.h"


template<typename T>
MultiConnectNode<T>* ColumnLinkPolicy<T>::do_next(const MultiConnectNode<T> *node) {
    return node->column_next();
}

template<typename T>
void ColumnLinkPolicy<T>::do_link_front(MultiConnectNode<T> *&head, MultiConnectNode<T> *node) {
    if (head == nullptr) {
        node->set_column_next(node);
        node->set_column_prev(node);
        head = node;
        return;
    }

    ColumnLinkPolicy::do_link_after(head->column_prev(), node);
    head = node;
}

template<typename T>
void ColumnLinkPolicy<T>::do_link_after(MultiConnectNode<T> *position, MultiConnectNode<T> *node) {
    MultiConnectNode<T> *next_node = position->column_next();

    node->set_column_prev(position);
    node->set_column_next(next_node);
    position->set_column_next(node);
    next_node->set_column_prev(node);
}

template<typename T>
void ColumnLinkPolicy<T>::do_unlink(MultiConnectNode<T> *&head, MultiConnectNode<T> *node) {
    MultiConnectNode<T> *next_node = node->column_next();

    if (next_node == node) {
        head = nullptr;
    } else {
        MultiConnectNode<T> *prev_node = node->column_prev();

        prev_node->set_column_next(next_node);
        next_node->set_column_prev(prev_node);

        if (head == node) {
            head = next_node;
        }
    }

    ColumnLinkPolicy::do_reset(node);
}

template<typename T>
void ColumnLinkPolicy<T>::do_reset(MultiConnectNode<T> *node) {
    node->set_column_next(nullptr);
    node->set_column_prev(nullptr);
}
