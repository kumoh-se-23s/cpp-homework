//
// Created by Merutilm on 10/29/26.
//

#pragma once
#include "CircularLinkedList.h"

struct TeamMemberCircularLinkedList : CircularLinkedList<TeamMemberCircularLinkedList, RAIISharedPointer<StudentNode>> {

    static RAIISharedPointer<StudentNode> &nextNode(RAIISharedPointer<StudentNode> &v) {
        return v->nextTeamMember;
    };

    static RAIISharedPointer<StudentNode> &prevNode(RAIISharedPointer<StudentNode> &v) {
        return v->prevTeamMember;
    };
};
