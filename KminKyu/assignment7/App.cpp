#include "App.h"
#include <iostream>
#include <sstream>

using namespace std;

int App::run(const char *argv[]) {
    char command = '\0';
    while (command != 'Q' || command == 'q') {
        cin >> command;
        switch (command) {
            case 'L':
            case 'l':
                cout << this-> controller;
            case 'i':
            case 'I':
                insert();
            case 'd':
            case 'D':
                remove();

        }
    }
    return 0;
}

bool App::loadFromFile(const char *filePath) {
    if (!fileIO.openInput(filePath)) {
        return false;
    }

    char line[LINE_BUFFER_SIZE];

    // 1번째 줄: 최저년도 최고년도 (스킵)
    if (!fileIO.readLine(line, LINE_BUFFER_SIZE)) {
        fileIO.closeInput();
        return true;
    }

    // 2번째 줄: 회원수 팀수
    int count = 0;
    int groupCount = 0;
    if (fileIO.readLine(line, LINE_BUFFER_SIZE)) {
        std::istringstream countStream(line);
        countStream >> count >> groupCount;
    }

    // 3번째 줄~: 회원수만큼 즉시 정렬 삽입
    for (int i = 0; i < count; ++i) {
        if (!fileIO.readLine(line, LINE_BUFFER_SIZE)) {
            break;
        }
        parseAndInsert(line);
    }

    fileIO.closeInput();
    return true;
}

void App::parseAndInsert(const char *line) {
    std::istringstream iss(line);
    int studentId = 0;
    char groupBuf[128] = {};
    char nameBuf[256] = {};

    if (!(iss >> studentId >> groupBuf)) {
        return;
    }

    iss >> std::ws;
    iss.getline(nameBuf, sizeof(nameBuf));

    controller.insertStudent(studentId, MyString(groupBuf), MyString(nameBuf));
}

void App::insert() {
    cin >> item;
        
    cin >> studentId;
    cin >> group();
    cin
}
void App::remove() {}
App::App(const char *argv) {
}


