#ifndef CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
#define CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
#include "CircularDoublyLinkedList.h"
#include "DoubleNode.h"
#include "MySet.h"
#include "SinglyLinkedList.h"
#include "Student.h"
#include "KeyList.h"

class StudentController {
    using GroupNode = DoubleNode<Student>;
    using GroupList = KeyList<MyString, CircularDoublyLinkedList<Student>>;
    using IdList = KeyList<int, SinglyLinkedList<GroupNode>>;

    MySet<IdList> idList;
    MySet<GroupList> groupList;

private:
    static int getYear(int studentId);
};


#endif //CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
