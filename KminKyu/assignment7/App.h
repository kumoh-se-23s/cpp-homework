#pragma once


#include "MyStringFileIO.h"
#include "StudentController.h"

class App {
    StudentController controller;
    MyStringFileIO fileIO;


public:
    explicit App(const char *argv);

    int run();

private:
    bool loadFromFile();
    bool saveToFile();
    void insertToFile(MyString &str));
    void insert();
    void remove();;
};

