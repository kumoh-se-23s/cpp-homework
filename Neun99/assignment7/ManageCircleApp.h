#pragma once
#include "StudentMultiList.h"

class ManageCircleApp {
public:
    void run(const char* file);
private:
    StudentMultiList studentList;
    bool isRunning;
    static constexpr int INPUT_ARR_SIZE = 3;

    void handleInput(char input);

    void readFile(const char* file);
    void insertStudent();
    void deleteStudent();
    void saveFile(const char* file) const;
};