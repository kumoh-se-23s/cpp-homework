#include "StudentLinkedList.h"

StudentLinkedList::StudentLinkedList() {
    head = nullptr;
    size = 0;
}

StudentLinkedList::~StudentLinkedList() {
    deleteAll(head);
}

//기능---------------------------------------
//학번 오름차순으로 학생 추가
void StudentLinkedList::addAscendly(int id, MyString group, MyString name) {
    StudentNode* newStudent = new StudentNode(id, group, name);
    //TODO: 그룹 addAscendly 호출
    //size 0이거나 첫 대상보다 id가 작으면 맨 앞
    if (size == 0 || id < head->getStudent().getId()) {
        addFirst(newStudent);
    } else { //아니면 내 자리 찾으러 탐색 시작
        bool isAdded = false;
        StudentNode *preNode = head;
        for (StudentNode *indexNode = head->getNextClassmate(); indexNode != nullptr; indexNode = indexNode->getNextClassmate()) {
            if (indexNode->getStudent().getId() > id) { //자리 찾았으면 끼워넣고 탈출
                isAdded = true;
                addAfter(newStudent, preNode);
                break;
            }
            preNode = indexNode;
        }
        if (!isAdded) //다 돌았는데 못 찾았으면 마지막
            addLast(newStudent);
    }
}

//private------------------------------------
void StudentLinkedList::addFirst(StudentNode *newStudent) {
    newStudent->setNextClassmate(head);
    head = newStudent;

}

void StudentLinkedList::addAfter(StudentNode *newStudent, StudentNode *preNode) {
    newStudent->setNextClassmate(preNode->getNextClassmate());
    preNode->setNextClassmate(newStudent);
}

void StudentLinkedList::deleteAll(StudentNode *node) {
    if (node != nullptr)
        deleteAll(node->getNextClassmate());
    //TODO: 그룹 removeStudent 호출
    delete node;
}
