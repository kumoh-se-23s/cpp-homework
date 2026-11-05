#pragma once

#include "SinglyLinkedList.h"
#include "CircularLinkedList.h"
#include "Student.h"

struct StudentNode {
    Student student;
    
    SinglyLinkedList<Student>* nextStudent = nullptr;
    CircularLinkedList<Student>* nextTeam = nullptr;
    CircularLinkedList<Student>* prevTeam = nullptr;
};
