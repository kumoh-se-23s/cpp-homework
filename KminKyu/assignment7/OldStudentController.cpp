#include <iostream>

#include "OldStudentController.h"


OldStudentController::OldStudentController() {
    studentGroupList = Group();
    studentIdList = Id();

    this->studentCount = 0;
}

OldStudentController::OldStudentController(int minYear, int maxYear, int studentCount, int groupCount) {
    studentGroupList = Group(groupCount);
    studentIdList = Id(std::min(studentCount, (maxYear - minYear + 1)));

    this->studentCount = 0;
}

OldStudentController::~OldStudentController() {
    clearNoGroupNode();
}

OldStudentController::OldStudentController(OldStudentController &&other) noexcept {
    this->studentIdList = std::move(other.studentIdList);
    this->studentGroupList = std::move(other.studentGroupList);
    this->studentCount = other.studentCount;

    other.studentCount = 0;
}

OldStudentController & OldStudentController::operator=(OldStudentController &&other) noexcept {
    if (this != &other) {
        clearNoGroupNode();
        this->studentIdList = std::move(other.studentIdList);
        this->studentGroupList = std::move(other.studentGroupList);
        this->studentCount = other.studentCount;

        other.studentCount = 0;
    }
    return *this;
}

void OldStudentController::insertStudent(int studentId, const OOldMyString &studentGroup, const OOldMyString &studentName) {
    int idKey = getYear(studentId);
    Student newStudent = Student(studentId, studentGroup, studentName);
    IdLinkedList *idLinkedList = studentIdList.getValue(idKey);
    CircularDoublyLinkedList<Student> *groupLinkedList = nullptr;

    if (studentGroup != "-") {
        groupLinkedList = studentGroupList.getValue(studentGroup);
    }

    //학번 중복여부 확인
    if (idLinkedList != nullptr) {
        DoubleNode<Student> searchNode = DoubleNode(newStudent);
        if (idLinkedList->find(NestedDoubleNode(&searchNode)) != nullptr) {
            return;
        }
    }

    DoubleNode<Student> *resultNode;

    //그룹을 찾을 수 없는 경우 / 존재하는 경우
    if (groupLinkedList == nullptr) {
        //그룹이 없는 경우 / 새로 추가하는 경우
        if (studentGroup == "-") {
            resultNode = new DoubleNode(newStudent);
        } else {
            CircularDoublyLinkedList<Student> *linkedList = new CircularDoublyLinkedList(new DoubleNode(newStudent));
            studentGroupList.insert(studentGroup, linkedList);
            resultNode = linkedList->getHead();
        }

    } else {
        resultNode = studentGroupList.getValue(studentGroup)->add(newStudent);
    }

    NestedDoubleNode<Student> nestedDoubleNode = NestedDoubleNode(resultNode);

    //존재하지않는 id인 경우 / 존재하는 경우
    if (idLinkedList == nullptr) {
        IdLinkedList *linkedList = new OldSinglyLinkedList(new OldSingleNode(nestedDoubleNode));
        studentIdList.insert(idKey, linkedList);
    } else {
        idLinkedList->add(nestedDoubleNode);
    }
    ++studentCount;

}

void OldStudentController::deleteStudent(int studentId) {
    int idKey = getYear(studentId);
    IdLinkedList *singleLinkedList = studentIdList.getValue(idKey);

    if (singleLinkedList == nullptr) {
        return;

    }
    DoubleNode<Student> searchNode = DoubleNode(Student(studentId, ""));


    IdNode *deleteIdNode = singleLinkedList->find(NestedDoubleNode(&searchNode));
    if (deleteIdNode == nullptr) {
        return;
    }
    //그룹이 없는경우 삭제할 그룹 노드
    const DoubleNode<Student> *targetDoubleNode = deleteIdNode->getItem().getNode();
    Student targetStudent = deleteIdNode->getItem().getUnravelItem();

    singleLinkedList->remove(deleteIdNode->getItem());

    //링크드리스트의 값이 비었을시 제거
    if (singleLinkedList->getSize() == 0) {
        studentIdList.removeByKey(idKey);
    }
    --studentCount;

    if (targetStudent.isGroupEmpty()) {
        delete targetDoubleNode;
        return;
    }

    CircularDoublyLinkedList<Student> *doubleLinkedList = studentGroupList.getValue(targetStudent.getStudentGroup());
    if (doubleLinkedList != nullptr) {
        doubleLinkedList->remove(targetStudent);
        if (doubleLinkedList->getSize() == 0) {
            studentGroupList.removeByKey(targetStudent.getStudentGroup());
        }
    }

}

OOldMyString OldStudentController::toString() const {
    OOldMyString result = OOldMyString();
    int lastIndex = studentIdList.getSize();
    if (lastIndex == 0) {
        result += "0 0";
    } else {
        result += OOldMyString::integerToString(studentIdList.at(0)->getKey());
        result += " ";
        result += OOldMyString::integerToString(studentIdList.at(lastIndex - 1)->getKey());
    }
    result += "\n";

    result += OOldMyString::integerToString(studentCount);
    result += " ";
    result += OOldMyString::integerToString(studentGroupList.getSize());
    result += "\n";
    result += studentIdList.toString();
    return result;
}

void OldStudentController::print(std::ostream &out) const {
    int lastIndex = studentIdList.getSize();
    out << "[Year:";
    if (lastIndex == 0) {
        out << "0~0";
    } else {
        out << OOldMyString::integerToString(studentIdList.at(0)->getKey());
        out << "~";
        out << OOldMyString::integerToString(studentIdList.at(lastIndex - 1)->getKey());
    }
    out << "] [Student Count:";
    out << OOldMyString::integerToString(studentCount);
    out << "] [Group Count: ";
    out << OOldMyString::integerToString(studentGroupList.getSize());
    out << "]\n";
    out << studentIdList;
}

int OldStudentController::getYear(int studentId) {
    return studentId / 10000;
}

//그룹이 없는 학생들은 종료 시 삭제가 되지 않음 -> 별도로 저장하여 삭제 or 소멸자에서 찾아서 삭제
void OldStudentController::clearNoGroupNode() {
    for (int index = 0; index < studentIdList.getSize(); ++index) {
        const IdLinkedList *list = studentIdList.at(index)->getValue();
        //리스트가 차있는 경우만 반복
        if (list != nullptr) {
            IdNode *now = list->getHead();
            while (now != nullptr) {
                const DoubleNode<Student> *item = now->getItem().getNode();
                if (item != nullptr && item->getItem().isGroupEmpty()) {
                    delete item;
                }
                now = now->getNext();
            }
        }
    }
}

std::ostream &operator<<(std::ostream &out, const NestedDoubleNode<Student> &nested) {
    const DoubleNode<Student> *doubleNode = nested.getNode();
    if (doubleNode == nullptr) {
        return out;
    }

    const Student &student = doubleNode->getItem();
    out << student << "(";
    if (!student.isGroupEmpty()) {
        out << "[" << student.getStudentGroup() << "]";
        const DoubleNode<Student> *now = doubleNode->getNext();
        while (now != nullptr && now != doubleNode) {
            out << " ";
            out << now->getItem();
            now = now->getNext();
            if (now != nullptr && now != doubleNode) {
                out << " -";
            }
        }
    }
    out << ")\n";
    return out;
}
std::ostream &operator<<(std::ostream &out, const OldStudentController &controller) {
    controller.print(out);
    return out;
}