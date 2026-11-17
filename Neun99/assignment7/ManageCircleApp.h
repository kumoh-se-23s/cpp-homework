#pragma once
#include "StudentMultiList.h"

class ManageCircleApp {
public:
    void run();
private:
    StudentMultiList studentList;
    bool isRunning;
    static constexpr int INPUT_ARR_SIZE = 3;

    // void readFile(); //TODO:파일스트림 인자
    void handleInput(char input);

    void insertStudent();
    void deleteStudent();
};