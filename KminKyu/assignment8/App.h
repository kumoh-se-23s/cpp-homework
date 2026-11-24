#pragma once

#include "MyStringFileIO.h"
#include "StudentController.h"

class App {
    StudentController controller;
    MyStringFileIO fileIO;


public:
    App();
    App(int argc, char* argv[]);

    void start();
    void run(char command);

private:
    bool loadFromFile();
    bool saveToFile();
    void insert();
    void search();
    void remove();
    void modify();
};

