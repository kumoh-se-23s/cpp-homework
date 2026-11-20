#pragma once
#include "StudentMultiList.h"

class ManageCircleApp {
public:
    void run(const char* file);
private:
    StudentMultiList studentList;
    bool isRunning;
    static constexpr int INPUT_ARR_SIZE = 3;
    static constexpr int ID_IDX = 0;
    static constexpr int GROUP_IDX = 1;
    static constexpr int NAME_IDX = 2;

    void handleInput(char input);

    void readFile(const char* file);
    void insertStudent();
    void deleteStudent();
    void saveFile(const char* file) const;
};