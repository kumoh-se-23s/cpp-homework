#pragma once
#include "NodeCompare.h"


template<typename T, CompareTraits<T> Traits>
bool NodeCompare<T, Traits>::less(const MultiConnectNode<T> *left, const MultiConnectNode<T> *right) {
    return Traits::less(left->get(), right->get());
}
