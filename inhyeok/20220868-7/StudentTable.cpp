//
// Created by Merutilm on 10/28/26.
//

#include "StudentTable.h"

#include "StudentSingleLinkedList.h"
#include "DoubleNode.h"
#include "TeamMemberCircularLinkedList.h"
#include <fstream>

StudentTable::StudentTable() {
}

StudentTable::~StudentTable() {
    //noop
}

void StudentTable::addStudent(Student student) {
    const RAIISharedPointer node(StudentNode(std::move(student)));
    bool inserted = false;
    const int year = student.getYear();
    const int teamId = student.getTeamId();
    auto studentIt = studentList.begin();
    for (; studentIt.hasNext(); ++studentIt) {
        const int key = (*studentIt)->data.key;
        if (key == year) {
            (*studentIt)->data.value.addFirst(node);
            inserted = true;
            break;
        }
        if (key > year) {
            //key not found
            KeyValuePair<int, StudentSingleLinkedList> pair;
            pair.key = year;
            pair.value.addFirst(node);
            auto ptr = RAIISharedPointer(DoubleNode(std::move(pair)));
            studentList.insertBefore(*studentIt, ptr);
            studentHashMap.put(year, ptr);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        KeyValuePair<int, StudentSingleLinkedList> pair;
        pair.key = year;
        pair.value.addFirst(node);
        auto ptr = RAIISharedPointer(DoubleNode(std::move(pair)));
        if (studentList.getSize() == 0) {
            studentList.addFirst(ptr);
        } else {
            studentList.insertAfter(*--studentIt, ptr);
        }
        studentHashMap.put(year, ptr);
    }

    inserted = false;
    auto teamIt = teamList.begin();
    for (; teamIt.hasNext(); ++teamIt) {
        const int key = (*teamIt)->data.key;
        if (key == teamId) {
            (*teamIt)->data.value.addFirst(node);
            inserted = true;
            break;
        }
        if (key > teamId) {
            KeyValuePair<int, TeamMemberCircularLinkedList> pair;
            pair.key = teamId;
            pair.value.addFirst(node);
            auto ptr = RAIISharedPointer(DoubleNode(std::move(pair)));
            teamList.insertBefore(*teamIt, ptr);
            teamHashMap.put(teamId, ptr);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        KeyValuePair<int, TeamMemberCircularLinkedList> pair;
        pair.key = teamId;
        pair.value.addFirst(node);
        auto ptr = RAIISharedPointer(DoubleNode(std::move(pair)));
        if (teamList.getSize() == 0) {
            teamList.addFirst(ptr);
        } else {
            teamList.insertAfter(*--teamIt, ptr);
        }
        teamHashMap.put(year, ptr);

    }
}

void StudentTable::removeStudent(const int num) {
    const int year = num / 10000;

    auto studentPair = studentHashMap[year];
    if (studentPair) {
        KeyValuePair<int, StudentSingleLinkedList> &entry = (*studentPair)->data;
        RAIISharedPointer<StudentNode> pn;
        for (auto it2 = entry.value.begin(); it2.hasNext(); ++it2) {
            if ((*it2)->data.getNum() == num) {
                pn = *it2;
                break;
            }
        }
        entry.value.remove(pn);
    }

    auto teamPair = teamHashMap[year];
    if (teamPair) {
        KeyValuePair<int, TeamMemberCircularLinkedList> &entry = (*teamPair)->data;
        for (auto it2 = entry.value.begin(); it2.hasNext(); ++it2) {
            if ((*it2)->data.getNum() == num) {
                entry.value.remove(*it2);
                break;
            }
        }
    }
}

MyString StudentTable::toString() const {
    MyString result;
    int min;
    int max;
    int studentCount;
    int teamCount;
    getStudentInfos(min, max, studentCount, teamCount);
    result.append("[Year:");
    result.append(MyString::toString(min));
    result.append("~");
    result.append(MyString::toString(max));
    result.append("] [Student Count:");
    result.append(MyString::toString(studentCount));
    result.append("] [Group Count:");
    result.append(MyString::toString(teamCount));
    result.append("]\n");

    for (auto itl = studentList.begin(); itl.hasNext(); ++itl) {
        for (auto it = (*itl)->data.value.begin(); it.hasNext(); ++it) {
            Student &student = (*it)->data;
            MyString d = student.toString();
            result += d.getData();
            const int teamId = student.getTeamId();

            if (teamId == Student::NO_GROUP) {
                result.append("()");
            } else {
                result.append("([G");
                result.append(MyString::toString(teamId));
                result.append("]");
                bool delim = false;
                auto teamPtr = &*it;
                do {
                    teamPtr = &(*teamPtr)->nextTeamMember;

                    if (*teamPtr != *it) {
                        if (delim) {
                            result.append(" - ");
                        } else {
                            result.append(" ");
                        }
                        result.append((*teamPtr)->data.toString());
                        delim = true;
                    }
                } while (*teamPtr != *it);

                result.append(")");
            }
            result.append('\n');
        }
    }

    return result;
}

void StudentTable::exportFile(const MyString &filename) const {
    std::ofstream out(filename.getData(), std::ios::trunc);
    int min;
    int max;
    int studentCount;
    int teamCount;
    getStudentInfos(min, max, studentCount, teamCount);

    out << min << " " << max << "\n" << studentCount << " " << teamCount << "\n";
    for (auto itl = studentList.begin(); itl.hasNext(); ++itl) {
        for (auto it = (*itl)->data.value.begin(); it.hasNext(); ++it) {
            Student &student = (*it)->data;
            out << student.toFormatString() << "\n";
        }
    }
}

void StudentTable::getStudentInfos(int &min, int &max, int &studentCount, int &teamCount) const {
    min = INT_MAX;
    max = INT_MIN;
    studentCount = 0;
    teamCount = teamList.getSize();

    if (teamList.getSize() > 0 && teamList.getHead()->data.key == Student::NO_GROUP) {
        --teamCount;
    }

    for (auto itl = studentList.begin(); itl.hasNext(); ++itl) {
        auto &value = (*itl)->data.value;
        studentCount += value.getSize();
        min = std::min(min, value.getSize() == 0 ? INT_MAX : (*itl)->data.key);
        max = std::max(max, value.getSize() == 0 ? INT_MIN : (*itl)->data.key);
    }
}
