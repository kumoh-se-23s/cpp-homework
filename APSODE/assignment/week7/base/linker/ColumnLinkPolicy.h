#ifndef COLUMN_LINK_POLICY_H
#define COLUMN_LINK_POLICY_H

#include "Policy.h"

template<typename T>
class ColumnLinkPolicy final : public Policy<T, ColumnLinkPolicy<T> > {
    public:
        static MultiConnectNode<T>* do_next(const MultiConnectNode<T> *node);

        static void do_link_front(MultiConnectNode<T> *&head, MultiConnectNode<T> *node);

        static void do_link_after(MultiConnectNode<T> *position, MultiConnectNode<T> *node);

        static void do_unlink(MultiConnectNode<T> *&head, MultiConnectNode<T> *node);

        static void do_reset(MultiConnectNode<T> *node);
};

#include "ColumnLinkPolicy.tpp"

#endif //COLUMN_LINK_POLICY_H
