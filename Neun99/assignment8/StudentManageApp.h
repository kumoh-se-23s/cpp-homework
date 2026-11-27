#pragma once
#include <iostream>
#include <fstream>
#include "MyLinkedList.hpp"
#include "AdStudent.h"

class StudentManageApp {
public:
    void run(const char* file);
private:
    bool isRunning;
    MyLinkedList<AdStudent> list;
    const MyString GIFTED = "Gifted";
    const MyString LIFELONG = "LifeLong";
    const MyString EMPLOYEE_EDU = "EmployeeEdu";
    const MyString SHORT_TERM_EDU = "ShortTermEdu";

    void readfile(const char* file);
    void savefile(const char* file);

    void printMenu();

    void viewAll() const;
    void search();
    void insert();
    void modify();
    void remove();

    AdStudent* createStudentWithFile(const MyString& type, ifstream& in);
    AdStudent* createStudentWithInput(const MyString& type, istream& in);
};