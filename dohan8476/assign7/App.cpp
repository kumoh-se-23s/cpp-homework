#include "App.h"
#include "FileIO.h"
#include <iostream>

using namespace std;

App::App(const char* file) : filename(file) {}

static char toUpper(char c) {
    if ('a' <= c && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;
}

static bool isDigit(const char c) {
    return '0' <= c && c <= '9';
}

static bool isAllDigits(const MyString& str) {
    for (int i = 0; i < str.length(); i++) {
        if (!isDigit(str[i])) {
            return false;
        }
    }
    return true;
}

//TODO
//일단은 8자리 가정하고 프로젝트 3에서 MyString stuID -> stoi(stuID.subStr(0,3))으로 년도 체크로 바꾸기
static bool isValidStudentID(const MyString& str) {
    if (str.length() != 8 || !isAllDigits(str)) return false;
    int year = stoi(str) / 10000;
    return 1900 <= year && year <= 2100;
}

void App::run() {
    if (!FileIO::loadFile(filename.getStr(), studentList)) {
        cout << "Error loading file." << endl;
        return;
    }

    MyString input;
    while (cin >> input) {
        // 길이 1이면 대문자 아님 널문자
        char cmd = input.length() == 1 ? toUpper(input[0]) : '\0';

        switch (cmd) {
            case 'L':
                cout << studentList;
                break;

            case 'I':
                handleInsert();
                break;

            case 'D':
                handleDelete();
                break;

            case 'Q':
                FileIO::saveFile(filename.getStr(), studentList);
                return;

            default:
                cout << input << " is not a valid command." << endl;
        }
    }
}

void App::handleInsert() {
    MyString stuID, teamID, name;

    cin >> stuID >> teamID;

    getline(cin, name);
    name.trim();

    if (isValidStudentID(stuID) && !name.isEmpty()) {
        studentList.addStudent(Student(stoi(stuID), name, teamID));
    }
}

void App::handleDelete() {
    MyString idStr;
    cin >> idStr;

    if (isValidStudentID(idStr)) {
        studentList.deleteStudent(stoi(idStr));
    }
}
