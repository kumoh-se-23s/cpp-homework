#ifndef NODE_COMPARE_H
#define NODE_COMPARE_H

#include "../list/linked_list/node/MultiConnectNode.h"
#include "../../core/TraitsConcept.h"

template<typename T, CompareTraits<T> Traits>
class NodeCompare {
    public:
        NodeCompare() = delete;

        static bool less(const MultiConnectNode<T> *left, const MultiConnectNode<T> *right);
};

#include "NodeCompare.tpp"

#endif //NODE_COMPARE_H
