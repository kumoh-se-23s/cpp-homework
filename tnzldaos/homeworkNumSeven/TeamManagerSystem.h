#pragma once
#include <optional>

#include "MyMultiLinkedList.h"
#include "Student.h";
using namespace std;

class TeamManagerSystem {
    string minStudentId;
    string maxStudentId;

    string memberNum;
    // string teamNum;

    MyMultiLinkedList<Student> myLinkedList = MyMultiLinkedList<Student>();

public:
    TeamManagerSystem();

    void run();

    void readFileToList(int argc, char *argv[]);

    optional<char> toUpper(char input);
};
