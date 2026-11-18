#ifndef CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
#define CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
#include "CircularDoublyLinkedList.h"
#include "MySet.h"
#include "SinglyLinkedList.h"
#include "Student.h"
#include "KeyArray.h"
#include "NestedSingleNode.h"

class StudentController {
    using GroupList = KeyArray<MyString, CircularDoublyLinkedList<Student>>;
    using IdList = KeyArray<int, SinglyLinkedList<NestedSingleNode<Student>>>;


private:
    static int getYear(int studentId);
};


#endif //CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
