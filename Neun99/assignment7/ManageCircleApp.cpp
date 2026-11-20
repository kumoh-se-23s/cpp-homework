#include "ManageCircleApp.h"
#include <fstream>
#include <limits>

void ManageCircleApp::run(const char *file) {
    readFile(file);

    isRunning = true;
    while (isRunning) {
        MyString input;
        getline(cin, input);
        if (input.length() == 1) //입력이 한글자일 때만
            handleInput(input[0]);
        else
            cout << "Invalid Input: not a char" << endl;
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
    cin >> inputArr[ID_IDX];
    cin >> inputArr[GROUP_IDX];
    getline(cin, inputArr[NAME_IDX]);

    if(!studentList.insertStudent(stoi(inputArr[ID_IDX]), inputArr[GROUP_IDX], inputArr[NAME_IDX].trim()))
        cout << "Insert failed: existing Id" << endl;
}

void ManageCircleApp::deleteStudent() {
    int userInput;
    cin >> userInput;
    if (cin.fail()) { //입력 이상하면 무시
        cin.clear();
        cout << "Invalid input." << endl;
    } else if (!studentList.removeStudent(userInput)) { //제거 시도, 성공하면 넘어가고 실패하면 안내
        cout << "Delete failed: invalid Id" << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //cin으로 받았으니 버퍼 청소
}

void ManageCircleApp::saveFile(const char *file) const {
    ofstream outStream;
    outStream.open(file);

    outStream << studentList.toStringForSave();
    outStream.close();
}
