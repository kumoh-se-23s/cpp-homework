#include "GroupHashNode.h"

GroupHashNode::GroupHashNode() {
    head = nullptr;
}

GroupHashNode::GroupHashNode(MyString group) {
    id = group;
    head = nullptr;
}

//getter------------------------------
MyString GroupHashNode::getId() const {
    return id;
}

StudentNode *GroupHashNode::getHead() const {
    return head;
}

//setter-----------------------------
void GroupHashNode::setId(MyString newId) {
    id = newId;
}

void GroupHashNode::setHead(StudentNode *newHead) {
    head = newHead;
}

//기능-----------------------------------
bool GroupHashNode::isEmpty() const {
    return head == nullptr;
}
