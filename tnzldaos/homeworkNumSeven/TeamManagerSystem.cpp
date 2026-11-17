#include "TeamManagerSystem.h"
#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

void TeamManagerSystem::run() {
    char input;

    bool isRun = true;
    while (isRun) {
        cin >> input;

        optional<char> upperString = toupper(input);

        if (upperString.has_value()) {
            switch (char inputResult = upperString.value()) {
                case 'L':

                case 'I':

                case 'D':

                case 'Q':

                default: ;
            }
        } else {
            cout << "잘못된 입력, 다시 입력해주세요";
        }
    }
}

void TeamManagerSystem::readFileToList(int argc, char *argv[]) {
    ifstream file(argv[1]);

    file >> minStudentId;
    file >> maxStudentId;
    file >> memberNum;

    string line;
    while (getline(file,line)) {
        map<string,string> inputMap;
        inputMap.insert()
    }

    // std.txt 예제의 학생 9명 (파일 순서 그대로: 학번, 이름, 팀ID)
    Student student1 = Student("20220987", "Harley Kim", "G1");
    Student student2 = Student("20231555", "William Jin", "G1");
    Student student3 = Student("20221004", "Thomas Lee", "G2");
    Student student4 = Student("19980123", "Jenny Anh", "G1");
    Student student5 = Student("19980909", "Liam Son", "G3");
    Student student6 = Student("19990131", "Owen Park", "G2");
    Student student7 = Student("19981475", "Aria Lee", "-");
    Student student8 = Student("20230554", "Oliver Kim", "G2");
    Student student9 = Student("20240178", "Nora Kang", "G1");

    myLinkedList.sortedInsert(student1);
    myLinkedList.sortedInsert(student2);
    myLinkedList.sortedInsert(student3);
    myLinkedList.sortedInsert(student4);
    myLinkedList.sortedInsert(student5);
    myLinkedList.sortedInsert(student6);
    myLinkedList.sortedInsert(student7);
    myLinkedList.sortedInsert(student8);
    myLinkedList.sortedInsert(student9);


}

optional<char> TeamManagerSystem::toUpper(char input) {
    if ('a' <= input && input <= 'z') {
        return static_cast<char>(input + 32);
    }

    if ('A' <= input && input <= 'Z') {
        return input;
    }

    return nullopt;
}



