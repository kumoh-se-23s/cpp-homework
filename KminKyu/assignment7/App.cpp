#include "App.h"
#include <iostream>
#include <fstream>

using namespace std;

App::App() : controller(), fileIO(nullptr) {}

App::App(int argc, char* argv[]) : fileIO(argc > 1 ? argv[1] : nullptr) {
    if (!loadFromFile()) {
        fileIO.closeRead();
    }
}

void App::start() {

    int result = 1;
    while (result != 0) {
        MyString command;
        if (!getline(cin, command)) {
            break;
        }
        result = run(command.trim());
        if (result == -1) {
            cout << "Invalid Input\n";
        }
    }
    saveToFile();
}

int App::run(MyString command) {
    if (command.length() != 1) {
        return -1;
    }
    switch (command.at(0)) {
        case 'L':
        case 'l':
            cout << this->controller;
            return 1;
        case 'i':
        case 'I':
            return insert();
        case 'd':
        case 'D':
            return remove();
        case 'Q':
        case 'q':
            return 0;
        default:
            return -1;
    }
}

bool App::loadFromFile() {
    if (!fileIO.openRead()) {
        controller = StudentController();
        return false;
    }

    ifstream &in = fileIO.getIn();
    MyString skip;

    int minYear = 0, maxYear = 0, studentCount = 0, groupCount = 0;

    //기본 정보가 실패하면 버퍼클리어
    if (!(in >> minYear >> maxYear)) {
        if (!in.eof()) {
            in.clear();
        } else {
            return false;
        }
    }
    getline(in, skip);

    if (!(in >> studentCount >> groupCount)){
        if (!in.eof()) {
            in.clear();
        } else {
            return false;
        }
    }
    getline(in, skip);

    controller = StudentController(minYear, maxYear, studentCount, groupCount);

    int studentId;
    MyString studentGroup;
    MyString studentName;

    while (!in.eof()) {
        if (in >> studentId >> studentGroup) {
            getline(in, studentName);
            controller.insertStudent(studentId, studentGroup, studentName.trim());
        } else {
            //파일이 종료된 상태가 아니면 버퍼 클리어
            if (!in.eof()) {
                in.clear();
                getline(in, skip);
            }
        }
    }

    fileIO.closeRead();
    return true;
}

bool App::saveToFile() {
    if (!fileIO.openWrite()) {
        return false;
    }

    fileIO.getOut() << controller.toString();
    fileIO.closeWrite();
    return true;
}

void App::insertToFile(MyString &str) {
    fileIO.writeLine(str);
}

int App::insert() {
    int studentId;

    if (!(cin >> studentId)) {
        if (!cin.eof()) {
            cin.clear();
            MyString skip;
            getline(cin, skip);
        }
        return -1;
    }

    MyString studentGroup;
    MyString studentName;

    cin >> studentGroup;
    getline(cin, studentName);

    controller.insertStudent(studentId, studentGroup, studentName.trim());
    return 1;
}

int App::remove() {
    int studentId;

    if (!(cin >> studentId)) {
        if (!cin.eof()) {
            cin.clear();
            MyString skip;
            getline(cin, skip);
        }
        return -1;
    }

    MyString clear;
    getline(cin, clear);
    controller.deleteStudent(studentId);

    return 1;
}