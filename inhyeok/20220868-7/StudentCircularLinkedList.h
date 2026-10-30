//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "CircularLinkedList.h"

struct StudentCircularLinkedList : CircularLinkedList<StudentCircularLinkedList, RAIISharedPointer<Node<Student>>> {

    static RAIISharedPointer<Node<Student>> &nextNode(RAIISharedPointer<Node<Student>> &v) {
        return v->sameYearNext;
    };

    static RAIISharedPointer<Node<Student>> &prevNode(RAIISharedPointer<Node<Student>> &v) {
        return v->sameYearPrev;
    };
};
