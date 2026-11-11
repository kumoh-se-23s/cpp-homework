//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "CircularLinkedList.h"

template<typename Data>
struct GenericCircularLinkedList : CircularLinkedList<GenericCircularLinkedList<Data>, RAIISharedPointer<DoubleNode<Data>>> {

    static RAIISharedPointer<DoubleNode<Data>> &nextNode(RAIISharedPointer<DoubleNode<Data>> &v) {
        return v->next;
    };

    static RAIISharedPointer<DoubleNode<Data>> &prevNode(RAIISharedPointer<DoubleNode<Data>> &v) {
        return v->prev;
    };
};

template<typename Data>
using MyList = GenericCircularLinkedList<Data>;

