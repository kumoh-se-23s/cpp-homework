#pragma once

#include "KeyValuePair.h"
#include "MyArrayList.h"
#include "MySinglyLinkedList.h"
#include "Node.h"
#include "Student.h"

class TeamManagerSystem {
    //연도별 리스트 -> 각 리스트가 그 연도 학생 노드의 주인
    MyArrayList<KeyValuePair<string, MySinglyLinkedList<Node<Student> > > > *yearArrayList = nullptr;
    //팀별 head -> 노드를 빌려 쓰기만 함 (지우지 않음)
    MyArrayList<KeyValuePair<string, Node<Student> > > *teamArrayList = nullptr;

    string fileName;

    bool linkToYearList(MyArrayList<KeyValuePair<string, MySinglyLinkedList<Node<Student> > > > *yearArrayList,
                        const string &year, Node<Student> *studentNode);

    void linkToTeamList(MyArrayList<KeyValuePair<string, Node<Student> > > *teamArrayList,
                        const string &teamName, Node<Student> *studentNode);

    void printStudent(Node<Student> *studentNode);

    Node<Student> *findStudentNode(const string &studentId);

    void countInformation(int &minYear, int &maxYear, int &studentCount, int &groupCount);

public:
    TeamManagerSystem() = default;

    ~TeamManagerSystem();

    void run(int argc, char *argv[]);

    void initializeProgram(int argc, char *argv[]);

    void printStudentList();

    void insertStudent(const string &studentId, const string &teamName, const string &studentName);

    void deleteStudent(const string &studentId);

    void saveFile();
};
