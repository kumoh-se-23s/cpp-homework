#include "App.h"
#include <iostream>
#include <fstream>

#include "AdStudent.h"

using namespace std;

App::App() : controller(), fileIO(nullptr) {}

App::App(int argc, char* argv[]) : fileIO(argc > 1 ? argv[1] : nullptr) {
    if (!loadFromFile()) {
        fileIO.closeRead();
    }
    cout << controller;
}

void App::start() {
    char command = '\0';
    MyString clear;
    while (command != '6') {
        if (!(cin >> command)) {
            cin.clear();
            getline(cin, clear);
        } else {
            run(command);
        }
    }
    saveToFile();
}

void App::run(char command) {
    switch (command) {
        case '1':
            cout << this->controller;
            break;
        case '2':
            search();
            break;
        case '3':
            insert();
            break;
        case '4':
            modify();
            break;
        case '5':
            remove();
            break;
        case '6':
            return;
        default:
            cout << "<ERROR> Invalid Input\n";
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

    fileIO.getOut() << controller;
    fileIO.closeWrite();
    return true;
}


void App::insert() {
    MyString studentType;

    if (!(cin >> studentType)) {
        if (!cin.eof()) {
            cin.clear();
            MyString skip;
            getline(cin, skip);
            cout << "<ERROR> Invalid Input\n";
        } else {
            return;
        }
    }

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