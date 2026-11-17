#include "ManageCircleApp.h"

void ManageCircleApp::run() {
    isRunning = true;
    while (isRunning) {
        char input;
        cin >> input;

        handleInput(input);
    }
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
    MyString userInput;
    getline(cin, userInput);

    MyString* inputArr = new MyString[INPUT_ARR_SIZE];
    userInput.split(' ', inputArr, INPUT_ARR_SIZE);

    studentList.insertStudent(stoi(inputArr[0]), inputArr[1], inputArr[2]);

    delete[] inputArr;
}

void ManageCircleApp::deleteStudent() {
    int userInput;
    cin >> userInput;

    if (!studentList.removeStudent(userInput))
        cout << "Delete failed: invalid Id" << endl;
}
