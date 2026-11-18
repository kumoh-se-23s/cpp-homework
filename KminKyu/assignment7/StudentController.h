#ifndef CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
#define CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
#include "CircularDoublyLinkedList.h"
#include "SinglyLinkedList.h"
#include "Student.h"
#include "LookUpTable.h"
#include "NestedSingleNode.h"

class StudentController {
    // using GroupList = LookUpTable<MyString, CircularDoublyLinkedList<Student>>;
    // using IdList = LookUpTable<int, SinglyLinkedList<NestedSingleNode<Student>>>;


private:
    static int getYear(int studentId);
};


#endif //CPP_HOMEWORK_GIT_STUDENTCONTROLLER_H
