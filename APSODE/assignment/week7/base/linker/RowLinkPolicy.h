#ifndef ROW_LINK_POLICY_H
#define ROW_LINK_POLICY_H

#include "Policy.h"

template<typename T>
class RowLinkPolicy final : public Policy<T, RowLinkPolicy<T> > {
    public:
        static MultiConnectNode<T>* do_next(const MultiConnectNode<T> *node);

        static void do_link_front(MultiConnectNode<T> *&head, MultiConnectNode<T> *node);

        static void do_link_after(MultiConnectNode<T> *position, MultiConnectNode<T> *node);

        static void do_unlink(MultiConnectNode<T> *&head, MultiConnectNode<T> *node);

        static void do_reset(MultiConnectNode<T> *node);
};

#include "RowLinkPolicy.tpp"

#endif //ROW_LINK_POLICY_H
