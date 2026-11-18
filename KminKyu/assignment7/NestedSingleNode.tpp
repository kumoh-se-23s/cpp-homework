#pragma once
#include "NestedSingleNode.h"

template<typename T>
const T& NestedSingleNode<T>::getUnravelItem() {
    return this->item.getItem();
}
