#include "StudentMultiList.h"

StudentMultiList::StudentMultiList() {
    idMap = IdHashMap();
    groupMap = GroupHashMap();
}

StudentMultiList::~StudentMultiList() {

}

void StudentMultiList::printAll() {

}

void StudentMultiList::insertStudent(int id, MyString group, MyString name) {
    StudentNode* newNode = new StudentNode(id, group, name);

    StudentNode* idHead = idMap[id / 10000];
    StudentNode* groupHead = groupMap[group];


}


//private-------------------------------------------------------------
//teammate포인터 처리: 이중 더블 연결리스트----------------------
//insert
//TODO
void StudentMultiList::teammateInsert(StudentNode *newNode) {
    if (groupMap[newNode->getStudent().getGroup()].isEmpty()) {
        groupHead = newNode;
        newNode->setNextTeammate(newNode);
        newNode->setPrevTeammate(newNode);
    } else {
        for ()
    }
}
