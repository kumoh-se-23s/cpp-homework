#include "ManageCircleApp.h"
#include <fstream>
#include <limits>

void ManageCircleApp::run(const char *file) {
    readFile(file);

    isRunning = true;
    while (isRunning) {
        char input;
        cin >> input;
        if (cin.peek() != EOF)
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        handleInput(input);
    }

    saveFile(file);
}

void ManageCircleApp::readFile(const char *file) {
    ifstream inStream;
    inStream.open(file);

    int minYear;
    int maxYear;
    int studentCnt;
    int groupCnt;

    inStream >> minYear >> maxYear;
    inStream >> studentCnt >> groupCnt;
    studentList.presetCapacities(maxYear - minYear + 1, groupCnt);
    MyString inputArr[INPUT_ARR_SIZE];

    for (int cnt = 0; cnt < studentCnt; cnt++) {
        inStream >> inputArr[0] >> inputArr[1];
        getline(inStream, inputArr[2]);
        studentList.insertStudent(stoi(inputArr[0]), inputArr[1], inputArr[2].trim());
    }

    inStream.close();
}

void ManageCircleApp::handleInput(char input) {
    switch (input) {
        //전체 출력
        case 'L':
        case 'l':
            cout << studentList.toStringForPrint() << endl;
            break;

        //학생 추가: 학번, 그룹, 이름 입력
        case 'I':
        case 'i':
            insertStudent();
            break;

        //학생 제거: 학번 입력
        case 'D':
        case 'd':
            deleteStudent();
            break;

        //종료
        case 'Q':
        case 'q':
            isRunning = false;
            break;

        default:
            cout << "Invalid Input" << endl;
            break;
    }
}

void ManageCircleApp::insertStudent() {
    MyString inputArr[INPUT_ARR_SIZE];
    cin >> inputArr[0];
    cin >> inputArr[1];
    getline(cin, inputArr[2]);

    if(!studentList.insertStudent(stoi(inputArr[0]), inputArr[1], inputArr[2].trim()))
        cout << "Insert failed: existing Id" << endl;
}

void ManageCircleApp::deleteStudent() {
    int userInput;
    cin >> userInput;

    if (!studentList.removeStudent(userInput))
        cout << "Delete failed: invalid Id" << endl;
}

void ManageCircleApp::saveFile(const char *file) const {
    ofstream outStream;
    outStream.open(file);

    outStream << studentList.toStringForSave();
    outStream.close();
}
