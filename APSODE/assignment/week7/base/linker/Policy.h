#ifndef CPP_HOMEWORK_POILICY_H
#define CPP_HOMEWORK_POILICY_H

#include "../list/linked_list/node/MultiConnectNode.h"

template<typename T, typename Derived>
class Policy {
    public:
        Policy() = delete;

        static MultiConnectNode<T>* next(const MultiConnectNode<T> *node);

        static void link_front(MultiConnectNode<T> *&head, MultiConnectNode<T> *node);

        static void link_after(MultiConnectNode<T> *position, MultiConnectNode<T> *node);

        static void unlink(MultiConnectNode<T> *&head, MultiConnectNode<T> *node);

        static void reset(MultiConnectNode<T> *node);
};

#include "Policy.tpp"

#endif //CPP_HOMEWORK_POILICY_H
