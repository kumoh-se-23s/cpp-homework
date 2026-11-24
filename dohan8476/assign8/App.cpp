#include "App.h"
#include "FileIO.h"
#include <iostream>

using namespace std;

App::App(const char* file) : filename(file) {}

void App::run() {
    if (!FileIO::loadFile(filename.getStr(), studentList)) {
        cout << "Error loading file." << endl;
        return;
    }

    cout << "******** AdStudent List ********\n";
    handleViewAll();

    int menu = 0;
    while (true) {
        cout << "[1] View all / [2] Search / [3] Insert New / [4] Modify / [5] Delete / [6] Exit: ";
        if (!(cin >> menu)) break;

        switch (menu) {
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

}

void App::handleModify() {

}

void App::handleSearch() const {

}

void App::handleInsert() {
}

void App::handleDelete() {
}


void App::handleExit() {

}