#pragma once
#include "StudentList.h"
#include "MyString.h"

class App {
    StudentList studentList;
    MyString filename;

    public:
        App(const char* file);
        void run();

        void handleInsert();
        void handleDelete();
};