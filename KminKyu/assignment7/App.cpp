#include "App.h"
#include <iostream>
#include <fstream>

using namespace std;

App::App() {
    controller = StudentController();

}

void App::start() {
    int result = 1;
    while (result != 0) {
        MyString command;
        getline(cin, command);
        result = run(command.trim());
        if (result == -1) {
            cout << "Invalid Input\n";
        }
    }
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
            insert();
            return 1;
        case 'd':
        case 'D':
            remove();
            return 1;
        case 'Q':
        case 'q':
            return 0;
        default:
            return -1;
    }
}

void App::insert() {
    int studentId;
    MyString studentGroup;
    MyString studentName;
    cin >> studentId;
    cin >> studentGroup;
    getline(cin,studentName);

    controller.insertStudent(studentId, studentGroup, studentName.trim());
}
void App::remove() {
    int studentId;
    cin >> studentId;
    MyString clear;
    getline(cin, clear);
    controller.deleteStudent(studentId);
}




