#include "StudentCircularList.h"

StudentCircularList::StudentCircularList() {
    head = nullptr;
    size = 0;
}

void StudentCircularList::addAscendly(int id, MyString name, MyString group) {
    if (size == 0) { //첫 원소인 경우
        head = new StudentNode(id, name, group);
        head->setNextTeammate(head); //원형연결
        head->setPrevTeammate(head);
        head->setNextClassmate(head);
    } else {
        for (int idx = 0; idx < size; idx++) {

        }
    }
}

void StudentCircularList::addAfter(int id, MyString name, MyString group, StudentNode *preNode) {

}


