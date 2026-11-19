#pragma once
#include "Poilicy.h"
#include "../util/RequireGuard.h"


template<typename T, typename Derived>
MultiConnectNode<T>* Policy<T, Derived>::next(const MultiConnectNode<T> *node) {
    require_non_null(node);

    return Derived::do_next(node);
}

template<typename T, typename Derived>
void Policy<T, Derived>::link_front(MultiConnectNode<T> *&head, MultiConnectNode<T> *node) {
    require_non_null(node);

    Derived::do_link_front(head, node);
}

template<typename T, typename Derived>
void Policy<T, Derived>::link_after(MultiConnectNode<T> *position, MultiConnectNode<T> *node) {
    require_non_null(position);
    require_non_null(node);

    Derived::do_link_after(position, node);
}

template<typename T, typename Derived>
void Policy<T, Derived>::unlink(MultiConnectNode<T> *&head, MultiConnectNode<T> *node) {
    require_non_null(head);
    require_non_null(node);

    Derived::do_unlink(head, node);
}

template<typename T, typename Derived>
void Policy<T, Derived>::reset(MultiConnectNode<T> *node) {
    require_non_null(node);

    Derived::do_reset(node);
}
