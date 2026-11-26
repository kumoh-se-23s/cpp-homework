#pragma once

#include "AdStudent.h"
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
    //파일에서 학생 정보를 불러오는 메소드
    bool loadFromFile();

    //파일으로 학생 정보를 저장하는 메소드
    bool saveToFile();

    void insert();

    void search();

    void remove();

    void modify();


    //학생 타입에 맞는 학생 객체 리턴
    AdStudent *getStudent(const MyString& type);
};

