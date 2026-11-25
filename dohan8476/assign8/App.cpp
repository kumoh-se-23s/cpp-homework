#include "App.h"
#include "FileIO.h"
#include "GiftedStudent.h"
#include "LifeLongStudent.h"
#include "EmployEduStudent.h"
#include "ShortTermEduStudent.h"
#include <iostream>

using namespace std;

App::App(const char* file) : filename(file) {}

void App::run() {
    if (!FileIO::loadFile(filename.getStr(), studentList)) {
        cout << "Error loading file." << endl;
        return;
    }

    cout << "*********************************************" << endl;
    cout << "--------- AdStudent List ---------" << endl;
    handleViewAll();

    int input = 0;
    while (true) {
        cout << "[1] View all / [2] Search / [3] Insert New / [4] Modify / [5] Delete / [6] Exit : ";
        if (!(cin >> input)) break;

        switch (input) {
            case 1: handleViewAll(); break;
            case 2: handleSearch(); break;
            case 3: handleInsert(); break;
            case 4: handleModify(); break;
            case 5: handleDelete(); break;
            case 6: handleExit(); return;
            default: break;
        }
    }
}

void App::handleViewAll() const {
    SinglyNode<AdStudent>* curr = studentList.getHead();
    while (curr != nullptr) {
        cout << curr->getData()->toString() << endl;
        curr = curr->getNext();
    }
}

void App::handleSearch() const {
    MyString targetCourse;
    //spec상 얘만 띄어쓰기
    cout << "Input Course : ";
    cin >> targetCourse;

    SinglyNode<AdStudent>* curr = studentList.getHead();
    while (curr != nullptr) {
        AdStudent* s = curr->getData();
        if (s->getCourse() == targetCourse) {
            cout << s->toString() << "\n";
        }
        curr = curr->getNext();
    }
}

void App::handleInsert() {
    MyString type;
    cout << "Input: ";
    cin >> type;

    AdStudent* student = nullptr;
    if (type == "Gifted") student = new GiftedStudent();
    else if (type == "LifeLong") student = new LifeLongStudent();
    else if (type == "EmployeeEdu") student = new EmployEduStudent();
    else if (type == "ShortTermEdu") student = new ShortTermEduStudent();
    else return;

    student->input();

    if (studentList.find(student->getID()) != nullptr) {
        cout << "<ERROR> The same ID already exists." << endl;
        delete student;
        return;
    }

    studentList.insertAscend(student);
    cout << "<INSERT> " << student->toString() << endl;
}

void App::handleModify(){
    MyString key;
    cout << "Input ID or Phone Number(XXX-XXXX-XXXX): ";
    cin >> key;

    AdStudent* target = studentList.remove(key);
    if (target == nullptr) {
        cout << "<ERROR> ID or Phone Number is invalid" << endl;
        return;
    }

    cout << "Current: " << target->toString() << endl;
    target->modify();

    // 전화번호 변경 시 동명이인 정렬 순서 유지를 위해 재삽입
    studentList.insertAscend(target);
    cout << "<UPDATE> " << target->toString() << "\n";
}

void App::handleDelete() {
    MyString key;
    cout << "Input ID or Phone Number(XXX-XXXX-XXXX): ";
    cin >> key;

    AdStudent* target = studentList.find(key);
    if (target == nullptr) {
        cout << "<ERROR> ID or Phone Number is invalid\n";
        return;
    }

    cout << target->toString() << " will be deleted. OK? [Y/N] ";
    char confirm;
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        AdStudent* removed = studentList.remove(key);
        cout << "<DELETE> " << removed->toString() << "\n";
        delete removed;
    }
}


void App::handleExit() const {
    cout << "W/w to save changes to " << filename << ", C/c otherwise: ";
    char cmd;
    cin >> cmd;

    if (cmd == 'W' || cmd == 'w') {
        FileIO::saveFile(filename.getStr(), studentList);
        cout << "Save changes and Exit\n";
    } else {
        cout << "Exit without Saving\n";
    }
}
