#include "StudentListManager.h"
#include "FileIO.h"
#include <iostream>

using namespace std;

StudentListManger::StudentListManger(const char* file) : filename(file) {}

static char toUpper(char c) {
    if ('a' <= c && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;
}

static bool isDigit(const char c) {
    return '0' <= c && c <= '9';
}

static bool isAllDigits(const MyString& s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isDigit(s[i])) {
            return false;
        }
    }
    return true;
}

static bool isValidStudentID(const MyString& s) {
    if (s.length() != 8 || !isAllDigits(s)) return false;
    int year = stoi(s) / 10000;
    return 1900 <= year && year <= 2100;
}

void StudentListManger::run() {
    if (!FileIO::loadFile(filename.getStr(), studentList)) {
        cout << "Error loading file." << endl;
        return;
    }

    MyString input;
    while (cin >> input) {
        if (input.length() != 1) continue;

        switch (toUpper(input[0] )) {
            case 'L':
                studentList.printList();
                break;

            case 'I': {
                MyString stuID, teamID, name;
                if (!(cin >> stuID >> teamID)) break;
                getline(cin, name);
                name.trim();

                if (isValidStudentID(stuID) && !name.isEmpty()) {
                    studentList.addStudent(Student(stoi(stuID), name, teamID));
                }
                break;
            }

            case 'D': {
                MyString idStr;
                if (!(cin >> idStr)) break;

                if (isValidStudentID(idStr)) {
                    studentList.deleteStudent(stoi(idStr));
                }
                break;
            }

            case 'Q':
                FileIO::saveFile(filename.getStr(), studentList);
                return;

            default:
                cout << input << " is not a valid command." << endl;
        }
    }
}

