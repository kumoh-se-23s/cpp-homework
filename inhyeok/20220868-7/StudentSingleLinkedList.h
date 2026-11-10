//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "SingleLinkedList.h"
#include "StudentNode.h"

struct StudentSingleLinkedList :SingleLinkedList<StudentSingleLinkedList, RAIISharedPointer<StudentNode>> {

    static RAIISharedPointer<StudentNode> &nextNode(RAIISharedPointer<StudentNode> &v) {
        return v->sameYearNext;
    };
};
