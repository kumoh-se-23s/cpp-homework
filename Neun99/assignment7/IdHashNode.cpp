#include "IdHashNode.h"

//생성자-------------------------------------
IdHashNode::IdHashNode() {
    id = -1;
    head = nullptr;
    size = 0;
}

IdHashNode::IdHashNode(int id) {
    this->id = id;
    head = nullptr;
    size = 0;
}

//getter-------------------------------------
int IdHashNode::getId() const{
    return id;
}

StudentNode *IdHashNode::getHead() const {
    return head;
}

//setter-------------------------------------
void IdHashNode::setId(int newId) {
    id = newId;
}

void IdHashNode::setHead(StudentNode *newHead) {
    head = newHead;
}

void IdHashNode::sizeUp() {
    size++;
}

void IdHashNode::sizeDown() {
    size--;
}

//기능----------------------------------------
bool IdHashNode::isEmtpy() const {
    return size == 0;
}
