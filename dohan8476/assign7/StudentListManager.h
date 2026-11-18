#pragma once
#include "StudentList.h"
#include "MyString.h"

class StudentListManger {
    StudentList studentList;
    MyString filename;

    public:
        StudentListManger(const char* file);
        void run();
};