#include "StudentNode.h"

StudentNode::StudentNode(int id, const MyString &group, const MyString &name) {
    student = Student(id, group, name);
}

//getter-----------------------------------------------
const Student& StudentNode::getStudent() const {
    return student;
}

StudentNode *StudentNode::getNextTeammate() const {
    return nextTeammate;
}

StudentNode *StudentNode::getPrevTeammate() const {
    return prevTeammate;
}

StudentNode *StudentNode::getNextClassmate() const {
    return nextClassmate;
}

//setter-----------------------------------------------
void StudentNode::setNextTeammate(StudentNode *newNextTeammate) {
    nextTeammate = newNextTeammate;
}

void StudentNode::setPrevTeammate(StudentNode *newPrevTeammate) {
    prevTeammate = newPrevTeammate;
}

void StudentNode::setNextClassmate(StudentNode *newNextClassmate) {
    nextClassmate = newNextClassmate;
}
