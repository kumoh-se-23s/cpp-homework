#include "StudentList.h"
#include <iostream>

using namespace std;

StudentList::StudentList() : studentCount(0) {}
StudentList::~StudentList() {}

void StudentList::initYearRange(int minY, int maxY) {
    yearTable.init(minY, maxY);
}

void StudentList::addStudent(const Student& std) {
    StudentNode* newNode = new StudentNode(std);
    // cout << "[New StudentNode] " << newNode << " (ID: " << std.getStudentID() << ")" << endl;

    // 두 테이블에 동일한 노드 전달 후 연결
    yearTable.insert(newNode); // next 연결
    teamTable.insert(newNode); // nextTeam, prevTeam 연결
    studentCount++;
}

bool StudentList::deleteStudent(int studentID) {
    // 연도 테이블에서 학번으로 찾아 분리
    StudentNode* target = yearTable.remove(studentID);
    if (target == nullptr) return false;

    // 팀 테이블에서 분리
    teamTable.remove(target);

    // cout << "[Delete StudentNode] " << target << " (ID: " << studentID << ")" << endl;
    delete target;
    studentCount--;
    return true;
}

void StudentList::printList() const {
    int minY = yearTable.getMinYear();
    int maxY = yearTable.getMaxYear();

    cout << "[Year:" << minY << "~" << maxY << "] "
         << "[Student Count:" << studentCount << "] "
         << "[Group Count: " << teamTable.getGroupCount() << "]" << endl;

    if (studentCount == 0 || minY > maxY) return;

    for (int year = minY; year <= maxY; ++year) {
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
