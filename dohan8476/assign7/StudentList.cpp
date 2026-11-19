#include "StudentList.h"
#include <iostream>

using namespace std;

StudentList::StudentList() : studentCount(0) {}
StudentList::~StudentList() {}

void StudentList::initYearRange(int minYear, int maxYear) {
    yearTable.init(minYear, maxYear);
}

void StudentList::addStudent(const Student& std) {
    if (yearTable.isExist(std.getStudentID())) {
        return;
    }

    int year = std.getYear();
    if (1900 <= year && year <= 2100) {
        StudentNode* newNode = new StudentNode(std);
        // cout << "[New StudentNode] " << newNode << " (ID: " << std.getStudentID() << ")" << endl;

        // 두 테이블에 동일한 노드 전달 후 연결
        yearTable.insert(newNode); // next 연결
        teamTable.insert(newNode); // nextTeam, prevTeam 연결
        studentCount++;
    }
}

bool StudentList::deleteStudent(int studentID) {
    // 연도 테이블에서 학번으로 찾아 분리=
    StudentNode* target = yearTable.remove(studentID);
    if (target == nullptr) return false;

    // 팀이 존재하면 테이블에서 분리
    if (target->data.hasTeam()) {
        teamTable.remove(target);
    }

    // cout << "[Delete StudentNode] " << target << " (ID: " << studentID << ")" << endl;
    delete target;
    studentCount--;
    return true;
}

void StudentList::printList() const {
    int minYear = yearTable.getMinYear();
    int maxYear = yearTable.getMaxYear();

    cout << "[Year:" << minYear << "~" << maxYear << "] "
         << "[Student Count:" << studentCount << "] "
         << "[Group Count: " << teamTable.getGroupCount() << "]" << endl;

    if (studentCount == 0 || minYear > maxYear) return;

    for (int year = minYear; year <= maxYear; ++year) {
        const SinglyLinkedList<StudentNode>& bucket = yearTable.getBucketByYear(year);
        StudentNode* curr = bucket.getHead();

        // 동일 연도 다음 학번
        while (curr != nullptr) {
            cout << curr->data.getStudentID() << " " << curr->data.getStudentName() << " (";

            if (curr->data.hasTeam()) {
                cout << "[" << curr->data.getTeamID() << "]";

                // 다음 팀원
                StudentNode* teamCurr = curr->nextTeam;
                while (teamCurr != nullptr && teamCurr != curr) {
                    cout << " " << teamCurr->data.getStudentID() << " " << teamCurr->data.getStudentName();
                    teamCurr = teamCurr->nextTeam;
                }
            }
            cout << ")" << endl;
            curr = curr->next;
        }
    }
}

int StudentList::getMinYear() const {
    return yearTable.getMinYear();
}

int StudentList::getMaxYear() const {
    return yearTable.getMaxYear();
}

int StudentList::getStudentCount() const {
    return studentCount;
}

int StudentList::getGroupCount() const {
    return teamTable.getGroupCount();
}

const YearHashTable &StudentList::getYearTable() const {
    return yearTable;
}
