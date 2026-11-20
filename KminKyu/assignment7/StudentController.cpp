#include "StudentController.h"
#include <iostream>

StudentController::StudentController() {
    studentGroupList = Group(DEFAULT_CAPACITY);
    studentIdList = Id(DEFAULT_CAPACITY);
}

StudentController::StudentController(int minYear, int maxYear, int studentCount, int groupCount) {
    studentGroupList = Group(groupCount);
    studentIdList = Id(std::min(studentCount, (maxYear - minYear + 1)));
}

void StudentController::insertStudent(int studentId, const MyString &studentGroup, const MyString &studentName) {
    int idKey = getYear(studentId);
    Student newStudent = Student(studentId, studentGroup, studentName);
    DoubleNode<Student> *resultNode;
    if (studentGroup == "-") {
        resultNode = new DoubleNode(newStudent);
    } else if (!studentGroupList.isEntry(studentGroup)) {

        CircularDoublyLinkedList<Student> *linkedList = new CircularDoublyLinkedList(new DoubleNode(newStudent));
        studentGroupList.insert(studentGroup, linkedList);
        resultNode = linkedList->getHead();
    } else {
        resultNode = studentGroupList.findEntry(studentGroup)->getValue()->add(newStudent);
    }
    NestedDoubleNode<Student> nestedDoubleNode = NestedDoubleNode(resultNode);

    if (!studentIdList.isEntry(idKey)) {
        IdLinkedList *linkedList = new SinglyLinkedList(new SingleNode(nestedDoubleNode));
        studentIdList.insert(idKey, linkedList);
    } else {
        studentIdList.findEntry(idKey)->getValue()->add(nestedDoubleNode);
    }

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

    if (targetStudent.isGroupEmpty()) {
        delete targetDoubleNode;
        return;
    }

    CircularDoublyLinkedList<Student> *doubleLinkedList = studentGroupList.getValue(targetStudent.getStudentGroup());
    if (doubleLinkedList != nullptr) {
        doubleLinkedList->remove(targetStudent);
    }

}

MyString StudentController::toString() const {
    return studentIdList.toString();
}

void StudentController::print(std::ostream &out) const {
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
    if (!student.isGroupEmpty() && doubleNode->getNext() != doubleNode) {
        out << " [" << student.getStudentGroup() << "] ";
        const DoubleNode<Student> *now = doubleNode->getNext();
        do {
            out << now->getItem();
            now = now->getNext();
            if (now != doubleNode) {
                out << " - ";
            }
        } while (now != doubleNode);
    }
    out << ")\n";
    return out;
}
std::ostream &operator<<(std::ostream &out, const StudentController &controller) {
    controller.print(out);
    return out;
}