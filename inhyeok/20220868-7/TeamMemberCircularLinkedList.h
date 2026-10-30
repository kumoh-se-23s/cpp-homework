//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "CircularLinkedList.h"

struct TeamMemberCircularLinkedList : CircularLinkedList<TeamMemberCircularLinkedList, RAIISharedPointer<Node<Student>>> {

    static RAIISharedPointer<Node<Student>> &nextNode(RAIISharedPointer<Node<Student>> &v) {
        return v->nextTeamMember;
    };

    static RAIISharedPointer<Node<Student>> &prevNode(RAIISharedPointer<Node<Student>> &v) {
        return v->prevTeamMember;
    };
};
