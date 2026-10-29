//
// Created by Merutilm on 10/28/26.
//

#include "StudentHashTable.h"

StudentHashTable::StudentHashTable(const int initialCapacity) {
    studentNumHashTableCapacity = initialCapacity;
}

StudentHashTable::~StudentHashTable() {
    delete[] studentTable;
    delete[] teamTable;
}

void StudentHashTable::addStudent(Student student) {
    addStudent(studentTable, studentNumHashTableCapacity, teamTable, teamNumHashTableCapacity, std::move(student));

    ++studentTableWeight;
    ++teamTableWeight;
}

void StudentHashTable::removeStudent(const Student &student) {
    const int studentHashIdx = hash(student.makeYearHash(), studentNumHashTableCapacity);
    const int teamNameHashIdx = hash(student.makeTeamHash(), teamNumHashTableCapacity);

    const RAIISharedStudentNode *studentNodeHeadPtr = &studentTable[studentHashIdx];
    const RAIISharedStudentNode *studentNodePtr = studentNodeHeadPtr;
    int rotation;
    if (studentNodePtr) {
        for (rotation = 0; studentNodePtr && studentNodeHeadPtr != studentNodePtr && (*studentNodePtr)->data != student;
             ++rotation) {
            studentNodePtr = &(*studentNodePtr)->sameYearNext;
        }

        if (!(rotation > 0 && studentNodeHeadPtr == studentNodePtr)) {
            const RAIISharedStudentNode &studentNode = *studentNodePtr;
            studentNode->sameYearPrev->sameYearNext = studentNode->sameYearNext;
            studentNode->sameYearNext->sameYearPrev = studentNode->sameYearPrev;
            --studentTableWeight;
        }
    }

    const RAIISharedStudentNode *teamNodeHeadPtr = &studentTable[teamNameHashIdx];
    const RAIISharedStudentNode *teamNodePtr = teamNodeHeadPtr;

    if (teamNodePtr) {
        for (rotation = 0; teamNodePtr && teamNodeHeadPtr != teamNodePtr && (*teamNodePtr)->data != student; ++
             rotation) {
            teamNodePtr = &(*teamNodePtr)->nextTeamMember;
        }
        if (!(rotation > 0 && teamNodeHeadPtr == teamNodePtr)) {
            const RAIISharedStudentNode &teamNode = *teamNodePtr;
            teamNode->prevTeamMember->nextTeamMember = teamNode->nextTeamMember;
            teamNode->nextTeamMember->prevTeamMember = teamNode->prevTeamMember;
            --teamTableWeight;
        }
    }
}

MyString StudentHashTable::toString() const {
    MyString result;
    int min;
    int max;
    getStudentMinMaxNum(min, max);
    result.append("[Year:");
    result.append(MyString::toString(min));
    result.append("~");
    result.append(MyString::toString(max));
    result.append("] [Student Count:");
    result.append(MyString::toString(studentTableWeight));
    result.append("] [Group Count:");
    result.append(MyString::toString(teamTableWeight));
    result.append("]\n");


    for (int i = 0; i < studentNumHashTableCapacity; ++i) {
        const auto studentHeadNode = &studentTable[i];
        auto studentCurrNode = studentHeadNode;
        if (!studentHeadNode) continue;

        do {
            Student &student = (*studentCurrNode)->data;
            MyString d = student.toString();
            result += d.getData();
            const int teamId = student.getTeamId();

            if (teamId == Student::NO_GROUP) {
                result.append("()");
            } else {
                result.append("([G");
                result.append(MyString::toString(teamId));

                const int teamHash = student.makeTeamHash();
                const auto teamHeadNode = &teamTable[teamHash];
                auto teamCurrNode = teamHeadNode;

                if (teamHeadNode) {
                    bool delim = false;
                    do {
                        if (delim) {
                            result.append(" - ");
                        }

                        if ((*teamCurrNode)->data != student) {
                            result.append((*teamCurrNode)->data.toString());
                        }
                        teamCurrNode = &(*teamCurrNode)->nextTeamMember;
                        delim = true;
                    } while (teamCurrNode != teamHeadNode);
                }
                result.append(")");
            }
            result.append('\n');

            studentCurrNode = &(*studentCurrNode)->sameYearNext;
        } while (studentCurrNode != studentHeadNode);
    }
    return result;
}

void StudentHashTable::getStudentMinMaxNum(int &min, int &max) const {
    min = INT_MAX;
    max = INT_MIN;
    for (int i = 0; i < studentNumHashTableCapacity; ++i) {
        const auto studentHeadNode = &studentTable[i];
        auto studentCurrNode = studentHeadNode;
        if (!studentHeadNode) continue;

        do {
            int year = (*studentCurrNode)->data.getYear();
            min = std::min(year, min);
            max = std::max(year, max);
            studentCurrNode = &(*studentCurrNode)->sameYearNext;
        } while (studentCurrNode != studentHeadNode);

    }
}

void StudentHashTable::resize(const int newStudentTableCapacity, const int newTeamTableCapacity) {
    auto newStudentTable = new RAIISharedStudentNode[newStudentTableCapacity];
    auto newTeamTable = new RAIISharedStudentNode[newTeamTableCapacity];
    for (int i = 0; i < studentNumHashTableCapacity; ++i) {
        auto head = &studentTable[i];
        auto curr = head;
        if (!head) continue;
        do {
            addStudent(newStudentTable, newStudentTableCapacity, newTeamTable, newTeamTableCapacity, (*curr)->data);
            curr = &(*curr)->sameYearNext;
        } while (curr != head);
    }

    delete[] studentTable;
    delete[] teamTable;
    studentTable = newStudentTable;
    teamTable = newTeamTable;
    studentNumHashTableCapacity = newStudentTableCapacity;
    teamNumHashTableCapacity = newTeamTableCapacity;
}

int StudentHashTable::hash(const int v, const int size) {
    return v % size;
}

void StudentHashTable::addStudent(RAIISharedStudentNode *currStudentTable, int currStudentNumHashTableCapacity,
                                  RAIISharedStudentNode *currTeamTable, int currTeamNumHashTableCapacity, Student student) {
    const RAIISharedStudentNode node(std::move(student));
    const int studentHashIdx = hash(node->data.makeYearHash(), currStudentNumHashTableCapacity);
    const int teamNameHashIdx = hash(node->data.makeTeamHash(), currTeamNumHashTableCapacity);
    resize(
        studentTableWeight >= currStudentNumHashTableCapacity / 2
            ? currStudentNumHashTableCapacity * 2
            : currStudentNumHashTableCapacity,
        teamTableWeight >= currTeamNumHashTableCapacity / 2
            ? currTeamNumHashTableCapacity * 2
            : currTeamNumHashTableCapacity
    );

    //assigning circular linked list
    const RAIISharedStudentNode &prevStudentHead = currStudentTable[studentHashIdx];

    if (prevStudentHead) {
        node->sameYearNext = prevStudentHead;
        node->sameYearPrev = prevStudentHead->sameYearPrev;
        prevStudentHead->sameYearPrev = node;
    } else {
        node->sameYearNext = node;
        node->sameYearPrev = node;
    }

    currStudentTable[studentHashIdx] = node;

    const RAIISharedStudentNode &prevTeamHead = currTeamTable[teamNameHashIdx];

    if (prevTeamHead) {
        node->nextTeamMember = prevTeamHead;
        node->prevTeamMember = prevTeamHead->prevTeamMember;
        prevTeamHead->prevTeamMember = node;
    } else {
        node->nextTeamMember = node;
        node->prevTeamMember = node;
    }
    currTeamTable[teamNameHashIdx] = node;
}
