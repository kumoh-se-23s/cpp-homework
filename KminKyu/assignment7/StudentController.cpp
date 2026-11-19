#include "StudentController.h"

StudentController::StudentController() {
    studentGroupList = Group(DEFAULT_CAPACITY);
    studentIdList = Id(DEFAULT_CAPACITY);
}

void StudentController::insertStudent(int studentId, const MyString &studentGroup, const MyString &studentName) {
    int idKey = getYear(studentId);
    Student newStudent = Student(studentId, studentGroup, studentName);
    DoubleNode<Student> *resultNode;
    if (studentGroup != "-") {
        resultNode = new DoubleNode(newStudent);
    } else if (studentGroupList.findEntry(studentGroup) != nullptr) {
        CircularDoublyLinkedList<Student> linkedList = CircularDoublyLinkedList(new DoubleNode(newStudent));
        resultNode = studentGroupList.insert(studentGroup, linkedList).getHead();
    } else {
        resultNode = studentGroupList.findEntry(studentGroup)->value.add(newStudent);
    }
    NestedDoubleNode<Student> nestedDoubleNode = NestedDoubleNode(resultNode);

    if (studentIdList.findEntry(idKey) != nullptr) {
        SinglyLinkedList<NestedDoubleNode<Student>> linkedList = SinglyLinkedList(new SingleNode(nestedDoubleNode));
        studentIdList.insert(idKey, linkedList);
    } else {
        studentIdList.findEntry(idKey)->value.add(nestedDoubleNode);
    }

}

void StudentController::deleteStudent(int studentId) {
    int idKey = getYear(studentId);

    if (studentIdList.findEntry(idKey) != nullptr) {
        SinglyLinkedList<NestedDoubleNode<Student>> singleLinkedList = studentIdList.getValue(idKey);
        NestedDoubleNode<Student> deleteStudentNode = singleLinkedList.find(Student(studentId, "", ""));
        CircularDoublyLinkedList<Student>
        DoubleNode<Student> *deleteNode = new DoubleNode(deleteStudent);


        linkedList.remove(NestedDoubleNode<Student>(deleteNode));
        if (){}
        delete deleteNode;
    }
}


MyString StudentController::toString() const {
    MyString result = MyString();
    for (int index = 0; index < studentIdList.getSize(); ++index) {
        result += studentIdList.getByIndex(index).toString();
    }

}

int StudentController::getYear(int studentId) {
    return studentId / 10000;
}
