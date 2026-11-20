#pragma once

#include "KeyValuePair.h"
#include "MyArrayList.h"
#include "MySinglyLinkedList.h"
#include "Node.h"
#include "Student.h"

class TeamManagerSystem {
    MyArrayList<KeyValuePair<string, MySinglyLinkedList<Node<Student> > > > yearsList;

    MyArrayList<KeyValuePair<string, Node<Student> > > *yearArrayList = nullptr;
    MyArrayList<KeyValuePair<string, Node<Student> > > *teamArrayList = nullptr;

    string fileName;

    void linkToYearList(MyArrayList<KeyValuePair<string, Node<Student> > > *yearArrayList,
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
