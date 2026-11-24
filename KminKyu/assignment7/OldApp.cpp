#include "OldApp.h"
#include <iostream>
#include <fstream>

using namespace std;

OldApp::OldApp() : controller(), fileIO(nullptr) {}

OldApp::OldApp(int argc, char* argv[]) : fileIO(argc > 1 ? argv[1] : nullptr) {
    if (!loadFromFile()) {
        fileIO.closeRead();
    }
}

void OldApp::start() {

    int result = 1;
    while (result != 0) {
        OOldMyString command;
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

int OldApp::run(OOldMyString command) {
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

bool OldApp::loadFromFile() {
    if (!fileIO.openRead()) {
        controller = OldStudentController();
        return false;
    }

    ifstream &in = fileIO.getIn();
    OOldMyString skip;

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

    controller = OldStudentController(minYear, maxYear, studentCount, groupCount);

    int studentId;
    OOldMyString studentGroup;
    OOldMyString studentName;

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

bool OldApp::saveToFile() {
    if (!fileIO.openWrite()) {
        return false;
    }

    fileIO.getOut() << controller.toString();
    fileIO.closeWrite();
    return true;
}


int OldApp::insert() {
    int studentId;

    if (!(cin >> studentId)) {
        if (!cin.eof()) {
            cin.clear();
            OOldMyString skip;
            getline(cin, skip);
        }
        return -1;
    }

    OOldMyString studentGroup;
    OOldMyString studentName;

    cin >> studentGroup;
    getline(cin, studentName);

    controller.insertStudent(studentId, studentGroup, studentName.trim());
    return 1;
}

int OldApp::remove() {
    int studentId;

    if (!(cin >> studentId)) {
        if (!cin.eof()) {
            cin.clear();
            OOldMyString skip;
            getline(cin, skip);
        }
        return -1;
    }

    OOldMyString clear;
    getline(cin, clear);
    controller.deleteStudent(studentId);

    return 1;
}