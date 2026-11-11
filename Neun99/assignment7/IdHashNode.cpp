#include "IdHashNode.h"

//생성자-------------------------------------
IdHashNode::IdHashNode() {
    id = -1;
    head = nullptr;
}

IdHashNode::IdHashNode(int id) {
    this->id = id;
    head = nullptr;
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

//기능----------------------------------------
bool IdHashNode::isEmtpy() const {
    return head == nullptr;
}
