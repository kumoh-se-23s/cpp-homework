#include <iostream>

#include "StudentController.h"


StudentController::StudentController() {
    studentGroupList = Group();
    studentIdList = Id();

    this->studentCount = 0;
}

StudentController::StudentController(int minYear, int maxYear, int studentCount, int groupCount) {
    studentGroupList = Group(groupCount);
    studentIdList = Id(std::min(studentCount, (maxYear - minYear + 1)));

    this->studentCount = 0;
}

void StudentController::insertStudent(int studentId, const MyString &studentGroup, const MyString &studentName) {
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
        IdLinkedList *linkedList = new SinglyLinkedList(new SingleNode(nestedDoubleNode));
        studentIdList.insert(idKey, linkedList);
    } else {
        idLinkedList->add(nestedDoubleNode);
    }
    ++studentCount;

}

void StudentController::deleteStudent(int studentId) {
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

MyString StudentController::toString() const {
    MyString result = MyString();
    int lastIndex = studentIdList.getSize();
    if (lastIndex == 0) {
        result += "0 0";
    } else {
        result += MyString::integerToString(studentIdList.at(0)->getKey());
        result += " ";
        result += MyString::integerToString(studentIdList.at(lastIndex - 1)->getKey());
    }
    result += "\n";

    result += MyString::integerToString(studentCount);
    result += " ";
    result += MyString::integerToString(studentGroupList.getSize());
    result += "\n";
    result += studentIdList.toString();
    return result;
}

void StudentController::print(std::ostream &out) const {
    int lastIndex = studentIdList.getSize();
    out << "[Year:";
    if (lastIndex == 0) {
        out << "0~0";
    } else {
        out << MyString::integerToString(studentIdList.at(0)->getKey());
        out << "~";
        out << MyString::integerToString(studentIdList.at(lastIndex - 1)->getKey());
    }
    out << "] [Student Count:";
    out << MyString::integerToString(studentCount);
    out << "] [Group Count: ";
    out << MyString::integerToString(studentGroupList.getSize());
    out << "]\n";
    out << studentIdList;
}

int StudentController::getYear(int studentId) {
    return studentId / 10000;
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
        while (now != doubleNode) {
            out << " ";
            out << now->getItem();
            now = now->getNext();
            if (now != doubleNode) {
                out << " -";
            }
        }
    }
    out << ")\n";
    return out;
}
std::ostream &operator<<(std::ostream &out, const StudentController &controller) {
    controller.print(out);
    return out;
}