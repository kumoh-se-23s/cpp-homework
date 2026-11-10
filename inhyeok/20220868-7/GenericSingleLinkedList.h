//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "SingleLinkedList.h"

template<typename Data>
struct GenericSingleLinkedList : SingleLinkedList<GenericSingleLinkedList<Data>, RAIISharedPointer<SingleNode<Data>>> {

    static RAIISharedPointer<SingleNode<Data>> &nextNode(RAIISharedPointer<SingleNode<Data>> &v) {
        return v->next;
    }
};
