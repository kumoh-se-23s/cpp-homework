#pragma once

#include "SinglyNode.h"
#include "DoubliyNode.h"
#include "Student.h"

struct StudentNode {
    Student student;
    
    SinglyNode<Student>* nextStudent = nullptr;
    DoublyNode<Student>* nextTeam = nullptr;
    DoublyNode<Student>* prevTeam = nullptr;
};
