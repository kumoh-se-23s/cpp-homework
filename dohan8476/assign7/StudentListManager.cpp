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

void StudentListManger::run() {
    if (!FileIO::loadFile(filename.getStr(), studentList)) {
        cout << "Error loading file." << endl;
        return;
    }

    char input;
    while (cin >> input) {
        input = toUpper(input);
        if (input == 'L') {
            studentList.printList();
        } 
        else if (input == 'I') {
            int id;
            MyString teamID;
            MyString name;

            cin >> id >> teamID;

            char c;
            while (cin.get(c) && (c == ' ' || c == '\t')) {}
            if (cin) cin.putback(c);

            getline(cin, name);
            if (!name.isEmpty() && name[name.length() - 1] == '\r') {
                name.erase(name.length() - 1, 1);
            }

            Student newStd(id, name, teamID);
            studentList.addStudent(newStd);
        } 
        else if (input == 'D') {
            int id;
            cin >> id;
            studentList.deleteStudent(id);
        } 
        else if (input == 'Q') {
            FileIO::saveFile(filename.getStr(), studentList);
            break;
        }
    }
}

