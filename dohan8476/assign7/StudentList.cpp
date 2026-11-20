#include "StudentList.h"
#include <iostream>

using namespace std;

StudentList::StudentList() : studentCount(0) {}
StudentList::~StudentList() {}

void StudentList::initYearRange(int minYear, int maxYear) {
    yearTable.init(minYear, maxYear);
}

void StudentList::addStudent(const Student& student) {
    // 중복 학번에 대해서는 Node 생성 X
    if (yearTable.isExist(student.getStudentID())) {
        return;
    }

    int year = student.getYear();
    if (1900 <= year && year <= 2100) {
        StudentNode* newNode = new StudentNode(student);
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

std::ostream& operator<<(std::ostream& os, const StudentList& studentList) {
    studentList.printList(os);
    return os;
}

void StudentList::printList(ostream& os) const {
    int minYear = getMinYear();
    int maxYear = getMaxYear();

    os << "[Year:" << minYear << "~" << maxYear << "] "
         << "[Student Count:" << studentCount << "] "
         << "[Group Count: " << teamTable.getGroupCount() << "]" << endl;

    if (studentCount == 0 || minYear > maxYear) return;

    for (int year = minYear; year <= maxYear; ++year) {
        const SinglyLinkedList<StudentNode>& bucket = yearTable.getBucketByYear(year);
        StudentNode* curr = bucket.getHead();

        // 동일 연도 다음 학번
        while (curr != nullptr) {
            os << curr->data.getStudentID() << " " << curr->data.getStudentName() << " (";

            if (curr->data.hasTeam()) {
                os << "[" << curr->data.getTeamID() << "]";

                // 다음 팀원
                StudentNode* teamCurr = curr->nextTeam;
                if (teamCurr != nullptr && teamCurr != curr) {
                    // 맨 처음 팀원은 공백만 출력
                    os << " " << teamCurr->data.getStudentID() << " " << teamCurr->data.getStudentName();
                    teamCurr = teamCurr->nextTeam;

                    while (teamCurr != nullptr && teamCurr != curr) {
                        os << " - " << teamCurr->data.getStudentID() << " " << teamCurr->data.getStudentName();
                        teamCurr = teamCurr->nextTeam;
                    }
                }
            }
            os << ")" << endl;
            curr = curr->next;
        }
    }
}

// StudentList.cpp
int StudentList::getMinYear() const {
    return studentCount > 0 ? yearTable.getMinYear() : 0;
}

int StudentList::getMaxYear() const {
    return studentCount > 0 ? yearTable.getMaxYear() : 0;
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
