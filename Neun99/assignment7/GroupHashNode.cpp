#include "GroupHashNode.h"

GroupHashNode::GroupHashNode() {
    head = nullptr;
    size = 0;
}

GroupHashNode::GroupHashNode(MyString group) {
    id = group;
    head = nullptr;
    size = 0;
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

void GroupHashNode::sizeUp() {
    size++;
}

void GroupHashNode::sizeDown() {
    size--;
}

//기능-----------------------------------
bool GroupHashNode::isEmpty() const {
    return size == 0;
}