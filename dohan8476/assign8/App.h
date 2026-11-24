#pragma once
#include "SinglyLinkedList.hpp"
#include "AdStudent.h"
#include "MyString.h"

class App {
    SinglyLinkedList<AdStudent> studentList;
    MyString filename;

    void handleViewAll() const;
    void handleSearch() const;
    void handleInsert();
    void handleModify();
    void handleDelete();
    void handleExit();

public:
    App(const char* file);
    void run();
};