//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "CircularLinkedList.h"

template<typename Data>
struct GenericCircularLinkedList : CircularLinkedList<GenericCircularLinkedList<Data>, RAIISharedPointer<Node<Data>>> {

    static RAIISharedPointer<Node<Data>> &nextNode(RAIISharedPointer<Node<Data>> &v) {
        return v->next;
    };

    static RAIISharedPointer<Node<Data>> &prevNode(RAIISharedPointer<Node<Data>> &v) {
        return v->prev;
    };
};
