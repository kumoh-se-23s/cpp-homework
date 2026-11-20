#include "StudentMultiList.h"

StudentMultiList::StudentMultiList() {
    studentCnt = 0;
}

//소멸자: StudentNode를 여기서 일괄 제거
StudentMultiList::~StudentMultiList() {
    //최소년도부터 최대년도까지 순회하며 delete
    for (int idx = idMap.getMinYear(); idx <= idMap.getMaxYear(); idx++) {
        deleteAllStudent(idMap[idx]->getHead());
    }
}

//기능-------------------------------------
// 용량 사전 확장
void StudentMultiList::presetCapacities(int yearCnt, int groupCnt) {
    idMap.presetCapacity(yearCnt);
    groupMap.presetCapacity(groupCnt);
}

// 파일 저장용 toString
MyString StudentMultiList::toStringForSave() const{
    MyString result;

    //최소년도, 최대년도
    result += itos(idMap.getMinYear()) + " " + itos(idMap.getMaxYear()) + "\n";

    //학생 수, 그룹 수
    result += itos(studentCnt) + " " + itos(groupMap.getSize());

    if (studentCnt == 0) //학생 없으면 바로 return
        return result;

    //최소년도부터 최대년도까지 순회
    for (int idx = idMap.getMinYear(); idx <= idMap.getMaxYear(); idx++) {
        //한 년도당 모든 학생 순회
        for (StudentNode* indexNode = idMap[idx]->getHead(); indexNode != nullptr; indexNode = indexNode->getNextClassmate()) {
            result += "\n";
            result += itos(indexNode->getStudent().getId()) + " ";
            result += indexNode->getStudent().getGroup() + " ";
            result += indexNode->getStudent().getName();
        }
    }
    return result;
}

//출력용 toString
MyString StudentMultiList::toStringForPrint() const{
    MyString result;

    //년도, 학생수, 그룹수
    result += "[Year:";
    result += itos(idMap.getMinYear()) + "~" + itos(idMap.getMaxYear());

    result += "] [Student Count:";
    result += itos(studentCnt);

    result += "] [Group Count:";
    result += itos(groupMap.getSize()) + "]\n";

    if (studentCnt == 0) //학생 없으면 바로 return
        return result;

    for (int idx = idMap.getMinYear(); idx <= idMap.getMaxYear(); idx++) {
        //한 년도당 모든 학생 순회
        for (StudentNode *indexNode = idMap[idx]->getHead(); indexNode != nullptr; indexNode = indexNode->getNextClassmate()) {
            result += indexNode->getStudent().toString();
            result += " (";

            //그룹이 있으면 자신 뒤부터 앞까지 출력
            if (indexNode->getStudent().getGroup() != NO_GROUP) {
                //그룹명
                result += "[";
                result += indexNode->getStudent().getGroup() + "]";

                //그룹원 정보
                StudentNode *groupIdxNode = indexNode->getNextTeammate();
                if (groupIdxNode != indexNode) { //팀에 혼자가 아니면 순회하며 전부 출력
                    result += " ";
                    result += groupIdxNode->getStudent().toString();
                    groupIdxNode = groupIdxNode->getNextTeammate();
                    for (; groupIdxNode != indexNode; groupIdxNode = groupIdxNode->getNextTeammate()) {
                        result += " - ";
                        result += groupIdxNode->getStudent().toString();
                    }
                }
            }
            result += ")\n"; //한 학생 끝
        }
    }

    return result;
}

//학생 추가
bool StudentMultiList::insertStudent(int id, const MyString &group, const MyString &name) {
    StudentNode* newNode = new StudentNode(id, group, name);
    if (!classmateInsert(newNode)) { //중복 id로 insert 실패 시 바로 false 리턴
        delete newNode;
        return false;
    }
    if (group != NO_GROUP)
        teammateInsert(newNode);
    studentCnt++;
    return true;
}

//학생 삭제
bool StudentMultiList::removeStudent(int id) {
    StudentNode* targetNode = classmateRemove(id);
    if (targetNode != nullptr) {
        studentCnt--;
        if (targetNode->getStudent().getGroup() != NO_GROUP)
            teammateRemove(targetNode);
    }
    delete targetNode;
    return targetNode != nullptr; //실제로 삭제되었는지 여부 리턴
}

// private-------------------------------------------------------------
int StudentMultiList::getYearOfId(int id) {
    return id / 10000;
}

// classmate포인터 처리: 단일 연결리스트-------------------------
//insert, 이미 존재하는 학생이면 추가하지 않고 false 리턴
bool StudentMultiList::classmateInsert(StudentNode *newNode) {
    int id = newNode->getStudent().getId();
    IdHashNode* idHash = idMap[getYearOfId(id)];
    if (idHash == nullptr)
        idHash = idMap.insert(getYearOfId(id));

    StudentNode* preNode = idHash->getHead();
    if (preNode == nullptr || preNode->getStudent().getId() > id) { //비었거나 첫 값이면 바로 넣기
        classmateInsertFront(idHash, newNode);
    } else if (preNode->getStudent().getId() == id) { //첫 값 비교, 같으면 false
        return false;
    } else { //아니면 순회 돌면서 자리 찾아 넣기
        for (;preNode->getNextClassmate() != nullptr; preNode = preNode->getNextClassmate()) {
            int compareId = preNode->getNextClassmate()->getStudent().getId();
            if (compareId == id) //이미 존재하면 false
                return false;
            if (compareId > id) //자리 찾으면 탈출
                break;
        }
        classmateInsertAfter(newNode, preNode);
    }
    return true;
}

//맨 앞에 연결 추가
void StudentMultiList::classmateInsertFront(IdHashNode* idHash, StudentNode *newNode) {
    newNode->setNextClassmate(idHash->getHead());
    idHash->setHead(newNode);
}

//인자 뒤에 연결 추가
void StudentMultiList::classmateInsertAfter(StudentNode *newNode, StudentNode *preNode) {
    newNode->setNextClassmate(preNode->getNextClassmate());
    preNode->setNextClassmate(newNode);
}

//remove
//인자 노드 찾아서 연결 제거하고 노드 주소 리턴
StudentNode *StudentMultiList::classmateRemove(int id) {
    IdHashNode* idHash = idMap[getYearOfId(id)];
    if (idHash == nullptr || idHash->isEmpty()) //없는 id면
        return nullptr;

    if (idHash->getHead()->getStudent().getId() == id) { //맨 앞이면
        StudentNode* targetNode = idHash->getHead();
        classmateRemoveFront(idHash);
        if (shouldRemoveHashNode(idHash))
            idMap.remove(getYearOfId(id));
        return targetNode;
    }

    StudentNode* preNode = idHash->getHead();
    for (; preNode->getNextClassmate() != nullptr; preNode = preNode->getNextClassmate()) {
        if (preNode->getNextClassmate()->getStudent().getId() == id) { //찾으면 제거
            StudentNode* targetNode = preNode->getNextClassmate();
            classmateRemoveAfter(preNode);
            if (shouldRemoveHashNode(idHash))
                idMap.remove(getYearOfId(id));
            return targetNode;
        }
    }
    return nullptr; //다 봐도 없는 id면
}

//맨 앞 연결 제거
StudentNode *StudentMultiList::classmateRemoveFront(IdHashNode *idHash) {
    StudentNode* temp = idHash->getHead();
    idHash->setHead(temp->getNextClassmate());
    return temp;
}

//인자 뒤 연결 제거
StudentNode *StudentMultiList::classmateRemoveAfter(StudentNode *preNode) {
    StudentNode* temp = preNode->getNextClassmate();
    preNode->setNextClassmate(preNode->getNextClassmate()->getNextClassmate());
    return temp;
}

bool StudentMultiList::shouldRemoveHashNode(const IdHashNode *idHash) const {
    return idHash->isEmpty() && (idHash->getId() == idMap.getMinYear() || idHash->getId() == idMap.getMaxYear());
}

//teammate포인터 처리: 이중 더블 연결리스트----------------------
//insert
void StudentMultiList::teammateInsert(StudentNode *newNode) {
    MyString group = newNode->getStudent().getGroup();
    GroupHashNode* groupHash = groupMap[group];
    if (groupHash == nullptr) //없는 그룹이면 새로 할당
        groupHash = groupMap.insert(group);

    if (groupHash->getHead() == nullptr) { //빈 리스트면 바로 넣기
        teammateInsertFirst(groupHash, newNode);
    } else { //원소가 있으면
        StudentNode* preNode = groupHash->getHead();
        int id = newNode->getStudent().getId();

        if (preNode->getStudent().getId() > id) { //맨 앞이면 헤드 변경 및 preNode 조정
            groupHash->setHead(newNode);
            preNode = preNode->getPrevTeammate();

        } else { //아니면 자기 자리 탐색
            for (;preNode->getNextTeammate() != groupHash->getHead() && preNode->getNextTeammate()->getStudent().getId() < id;
            preNode = preNode->getNextTeammate()) {} //탐색하다가 자기 자리 이전에서 멈춤
        }

        teammateInsertAfter(newNode, preNode); //이후 insert
    }
}

//첫 원소 추가
void StudentMultiList::teammateInsertFirst(GroupHashNode *groupHash, StudentNode *newNode) {
    groupHash->setHead(newNode);
    newNode->setNextTeammate(newNode);
    newNode->setPrevTeammate(newNode);
}

//인자로 받은 노드 뒤에 추가
void StudentMultiList::teammateInsertAfter(StudentNode *newNode, StudentNode *preNode) {
    preNode->getNextTeammate()->setPrevTeammate(newNode);
    newNode->setPrevTeammate(preNode);
    newNode->setNextTeammate(preNode->getNextTeammate());
    preNode->setNextTeammate(newNode);
}

//remove:: StudentNode 삭제는 위에서 하고 연결만 끊기
void StudentMultiList::teammateRemove(const StudentNode *targetNode) {
    MyString group = targetNode->getStudent().getGroup();
    GroupHashNode* groupHash = groupMap[group];

    if (groupHash->getHead() == groupHash->getHead()->getNextTeammate()) //원소가 하나면 헤드만 제거
        groupHash->setHead(nullptr);
    else {
        //맨 앞이면 head 변경
        if (groupHash->getHead() == targetNode)
            groupHash->setHead(targetNode->getNextTeammate());

        //이후 포인터 정리
        targetNode->getPrevTeammate()->setNextTeammate(targetNode->getNextTeammate());
        targetNode->getNextTeammate()->setPrevTeammate(targetNode->getPrevTeammate());
    }
    if (groupHash->isEmpty())
        groupMap.remove(group);
}

//모든 동기 노드 delete
void StudentMultiList::deleteAllStudent(const StudentNode *node) {
    if (node != nullptr)
        deleteAllStudent(node->getNextClassmate());
    delete node;
}
