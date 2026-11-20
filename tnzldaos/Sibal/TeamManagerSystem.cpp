#pragma once

#include "TeamManagerSystem.h"

#include <fstream>
#include <iostream>
#include <string>

#include "KeyValuePair.h"
#include "MyArrayList.h"
#include "MyDoublyLinkedList.h"
#include "Node.h"
#include "Student.h"
using namespace std;

TeamManagerSystem::~TeamManagerSystem() {
    if (yearArrayList != nullptr) {
        for (int index = 0; index < yearArrayList->getSize(); index++) {
            Node<Student> *current = yearArrayList->get(index).getValue();

            while (current != nullptr) {
                Node<Student> *next = current->getNextNode();

                delete current;

                current = next;
            }
        }

        delete yearArrayList;
        yearArrayList = nullptr;
    }

    if (teamArrayList != nullptr) {
        delete teamArrayList;
        teamArrayList = nullptr;
    }
}

void TeamManagerSystem::run(int argc, char *argv[]) {
    initializeProgram(argc, argv);

    char inputMenu;

    while (cin >> inputMenu) {
        switch (inputMenu) {
            case 'L':
                printStudentList();
                break;

            case 'I': {
                string studentId;
                string teamName;
                string studentName;

                cin >> studentId >> teamName;
                getline(cin, studentName);

                while (!studentName.empty() && studentName[0] == ' ') {
                    studentName.erase(0, 1);
                }

                insertStudent(studentId, teamName, studentName);
                break;
            }

            case 'D': {
                string studentId;
                cin >> studentId;

                deleteStudent(studentId);
                break;
            }

            case 'Q':
                saveFile();
                return;

            default:
                break;
        }
    }
}

void TeamManagerSystem::initializeProgram(int argc, char *argv[]) {
    if (argc < 2) {
        return;
    }

    fileName = argv[1];

    ifstream file(argv[1]);
    if (!file.is_open()) {
        return;
    }

    yearArrayList = new MyArrayList<KeyValuePair<string, Node<Student> > >();
    teamArrayList = new MyArrayList<KeyValuePair<string, Node<Student> > >();

    string line;

    //년,팀 갯수 읽기
    getline(file, line);
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        int secondSpace = line.find(' ', 8 + 1);

        string studentId = line.substr(0, 8);
        string teamName = line.substr(8 + 1, secondSpace - 8 - 1);
        string studentName = line.substr(secondSpace + 1);

        insertStudent(studentId, teamName, studentName);
    }
}

//년도 ArrayList 연결
void TeamManagerSystem::linkToYearList(MyArrayList<KeyValuePair<string, Node<Student> > > *yearArrayList,
                                       const string &year, Node<Student> *studentNode) {
    Comparator<Student> comparator;

    KeyValuePair<string, Node<Student> > *keyValue = yearArrayList->findObject(year);

    if (keyValue != nullptr) {
        //헤더를 가져옴
        Node<Student> *headNode = keyValue->getValue();

        //헤더를 가져와서 넣음으로써 SinglyLinkedList 만듦
        MySinglyLinkedList<Node<Student> > studentIdList = MySinglyLinkedList<Node<Student> >(headNode);
        studentIdList.sortedInsert(studentNode, comparator);

        keyValue->setValue(studentIdList.getHead());
    } else {
        KeyValuePair<string, Node<Student> > newKeyValue = KeyValuePair<string, Node<Student> >(year, studentNode);
        yearArrayList->append(newKeyValue);
    }
}

void TeamManagerSystem::linkToTeamList(MyArrayList<KeyValuePair<string, Node<Student> > > *teamArrayList,
                                       const string &teamName, Node<Student> *studentNode) {
    Comparator<Student> comparator;

    KeyValuePair<string, Node<Student> > *keyValue = teamArrayList->findObject(teamName);

    if (keyValue != nullptr) {
        Node<Student> *headNode = keyValue->getValue();

        MyDoublyLinkedList<Node<Student> > teamMemberList = MyDoublyLinkedList<Node<Student> >(headNode);
        teamMemberList.sortedInsert(studentNode, comparator);

        keyValue->setValue(teamMemberList.getHead());
    } else {
        KeyValuePair<string, Node<Student> > newKeyValue = KeyValuePair<string, Node<Student> >(teamName, studentNode);
        teamArrayList->append(newKeyValue);
    }
}

void TeamManagerSystem::printStudentList() {
    if (yearArrayList == nullptr || teamArrayList == nullptr) {
        return;
    }

    int minYear = 0;
    int maxYear = 0;
    int studentCount = 0;
    int groupCount = 0;

    countInformation(minYear, maxYear, studentCount, groupCount);

    cout << "[Year:" << minYear << "~" << maxYear << "] [Student Count:" << studentCount
            << "] [Group Count: " << groupCount << "]" << endl;

    for (int year = minYear; year <= maxYear; year++) {
        KeyValuePair<string, Node<Student> > *keyValue = yearArrayList->findObject(to_string(year));

        if (keyValue == nullptr) {
            continue;
        }

        for (Node<Student> *current = keyValue->getValue(); current != nullptr; current = current->getNextNode()) {
            printStudent(current);
        }
    }
}

void TeamManagerSystem::printStudent(Node<Student> *studentNode) {
    const Student &student = studentNode->getObject();

    cout << student.getId() << " " << student.getName() << " (";

    if (student.getTeamName() != "-") {
        cout << "[" << student.getTeamName() << "]";

        Node<Student> *teamHead = teamArrayList->findObject(student.getTeamName())->getValue();
        bool isFirst = true;

        for (Node<Student> *current = studentNode->getNextTeamMember(); current != nullptr;
             current = current->getNextTeamMember()) {
            cout << (isFirst ? " " : " - ") << current->getObject().getId() << " " << current->getObject().getName();
            isFirst = false;
        }

        for (Node<Student> *current = teamHead; current != studentNode; current = current->getNextTeamMember()) {
            cout << (isFirst ? " " : " - ") << current->getObject().getId() << " " << current->getObject().getName();
            isFirst = false;
        }
    }

    cout << ")" << endl;
}

Node<Student> *TeamManagerSystem::findStudentNode(const string &studentId) {
    if (yearArrayList == nullptr || studentId.size() < 4) {
        return nullptr;
    }

    KeyValuePair<string, Node<Student> > *keyValue = yearArrayList->findObject(studentId.substr(0, 4));

    if (keyValue == nullptr) {
        return nullptr;
    }

    for (Node<Student> *current = keyValue->getValue(); current != nullptr; current = current->getNextNode()) {
        if (current->getObject().getId() == studentId) {
            return current;
        }
    }

    return nullptr;
}

void TeamManagerSystem::insertStudent(const string &studentId, const string &teamName, const string &studentName) {
    if (studentId.size() < 4) {
        return;
    }

    if (yearArrayList == nullptr) {
        yearArrayList = new MyArrayList<KeyValuePair<string, Node<Student> > >();
    }
    if (teamArrayList == nullptr) {
        teamArrayList = new MyArrayList<KeyValuePair<string, Node<Student> > >();
    }

    if (findStudentNode(studentId) != nullptr) {
        return;
    }

    Student *student = new Student(studentId, teamName, studentName);
    Node<Student> *studentNode = new Node<Student>(student);

    linkToYearList(yearArrayList, studentId.substr(0, 4), studentNode);
    linkToTeamList(teamArrayList, teamName, studentNode);
}

void TeamManagerSystem::deleteStudent(const string &studentId) {
    if (yearArrayList == nullptr || teamArrayList == nullptr || studentId.size() < 4) {
        return;
    }

    string year = studentId.substr(0, 4);

    KeyValuePair<string, Node<Student> > *yearKeyValue = yearArrayList->findObject(year);

    if (yearKeyValue == nullptr) {
        return;
    }

    MySinglyLinkedList<Node<Student> > studentIdList = MySinglyLinkedList<Node<Student> >(yearKeyValue->getValue());

    Node<Student> *removedNode = studentIdList.remove(studentId);

    if (removedNode == nullptr) {
        return;
    }

    if (studentIdList.getHead() == nullptr) {
        yearArrayList->remove(year);
    } else {
        yearKeyValue->setValue(studentIdList.getHead());
    }

    string teamName = removedNode->getObject().getTeamName();

    KeyValuePair<string, Node<Student> > *teamKeyValue = teamArrayList->findObject(teamName);

    if (teamKeyValue != nullptr) {
        MyDoublyLinkedList<Node<Student> > teamMemberList = MyDoublyLinkedList<Node<
            Student> >(teamKeyValue->getValue());

        teamMemberList.remove(removedNode);

        if (teamMemberList.getHead() == nullptr) {
            teamArrayList->remove(teamName);
        } else {
            teamKeyValue->setValue(teamMemberList.getHead());
        }
    }

    delete removedNode;
}

void TeamManagerSystem::countInformation(int &minYear, int &maxYear, int &studentCount, int &groupCount) {
    minYear = 0;
    maxYear = 0;
    studentCount = 0;
    groupCount = 0;

    if (yearArrayList == nullptr || teamArrayList == nullptr) {
        return;
    }

    for (int index = 0; index < yearArrayList->getSize(); index++) {
        KeyValuePair<string, Node<Student> > &keyValue = yearArrayList->get(index);
        int year = stoi(keyValue.getKey());

        if (index == 0 || year < minYear) {
            minYear = year;
        }
        if (index == 0 || year > maxYear) {
            maxYear = year;
        }

        for (Node<Student> *current = keyValue.getValue(); current != nullptr; current = current->getNextNode()) {
            studentCount++;
        }
    }

    for (int index = 0; index < teamArrayList->getSize(); index++) {
        if (teamArrayList->get(index).getKey() != "-") {
            groupCount++;
        }
    }
}

void TeamManagerSystem::saveFile() {
    if (fileName.empty() || yearArrayList == nullptr || teamArrayList == nullptr) {
        return;
    }

    ofstream file(fileName);
    if (!file.is_open()) {
        return;
    }

    int minYear = 0;
    int maxYear = 0;
    int studentCount = 0;
    int groupCount = 0;

    countInformation(minYear, maxYear, studentCount, groupCount);

    file << minYear << " " << maxYear << endl;
    file << studentCount << " " << groupCount << endl;

    for (int year = minYear; year <= maxYear; year++) {
        KeyValuePair<string, Node<Student> > *keyValue = yearArrayList->findObject(to_string(year));

        if (keyValue == nullptr) {
            continue;
        }

        for (Node<Student> *current = keyValue->getValue(); current != nullptr; current = current->getNextNode()) {
            const Student &student = current->getObject();

            file << student.getId() << " " << student.getTeamName() << " " << student.getName() << endl;
        }
    }
}
