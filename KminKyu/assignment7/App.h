#pragma once


#include "MyStringFileIO.h"
#include "StudentController.h"

class App {
    StudentController controller;
    // MyStringFileIO fileIO;


public:
    App();
    // App(const char *argv);
    void start();
    int run(MyString command);

private:
    bool loadFromFile();
    bool saveToFile();
    void insertToFile(MyString &str);
    void insert();
    void remove();;
};

