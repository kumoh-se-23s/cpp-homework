//
// Created by Merutilm on 10/28/26.
//

#include "StudentTable.h"

#include "StudentCircularLinkedList.h"
#include "Node.h"
#include "TeamMemberCircularLinkedList.h"

StudentTable::StudentTable() {
}

StudentTable::~StudentTable() {
    //noop
}

void StudentTable::addStudent(Student student) {
    const RAIISharedPointer node(Node(std::move(student)));
    bool inserted = false;
    const int year = student.getYear();
    const int teamId = student.getTeamId();
    auto studentIt = studentTable.begin();
    for (; studentIt.hasNext(); ++studentIt) {
        const int key = (*studentIt)->data.key;
        if (key == year) {
            (*studentIt)->data.value.addFirst(node);
            inserted = true;
            break;
        }
        if (key > year) {
            KeyValuePair<int, StudentCircularLinkedList> pair;
            pair.key = year;
            pair.value.addFirst(node);
            auto ptr = RAIISharedPointer(Node(std::move(pair)));
            studentTable.insertBefore(*studentIt, ptr);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        KeyValuePair<int, StudentCircularLinkedList> pair;
        pair.key = year;
        pair.value.addFirst(node);
        auto ptr = RAIISharedPointer(Node(std::move(pair)));
        if (studentTable.getSize() == 0) {
            studentTable.addFirst(ptr);
        } else {
            studentTable.insertAfter(*--studentIt, ptr);
        }
    }

    inserted = false;
    auto teamIt = teamTable.begin();
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
            auto ptr = RAIISharedPointer(Node(std::move(pair)));
            teamTable.insertBefore(*teamIt, ptr);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        KeyValuePair<int, TeamMemberCircularLinkedList> pair;
        pair.key = teamId;
        pair.value.addFirst(node);
        auto ptr = RAIISharedPointer(Node(std::move(pair)));
        if (teamTable.getSize() == 0) {
            teamTable.addFirst(ptr);
        } else {
            teamTable.insertAfter(*--teamIt, ptr);
        }
    }
}

void StudentTable::removeStudent(const int num) {
    const int year = num / 10000;

    for (auto it = studentTable.begin(); it.hasNext(); ++it) {
        KeyValuePair<int, StudentCircularLinkedList> &entry = (*it)->data;
        if (entry.key == year) {
            for (auto it2 = entry.value.begin(); it2.hasNext(); ++it2) {
                if ((*it2)->data.getNum() == num) {
                    entry.value.remove(*it2);
                    break;
                }
            }
        }
    }

    for (auto it = teamTable.begin(); it.hasNext(); ++it) {
        KeyValuePair<int, TeamMemberCircularLinkedList> &entry = (*it)->data;
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

    for (auto itl = studentTable.begin(); itl.hasNext(); ++itl) {
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

void StudentTable::getStudentInfos(int &min, int &max, int &studentCount, int &teamCount) const {
    min = INT_MAX;
    max = INT_MIN;
    studentCount = 0;
    teamCount = 0;

    for (auto itl = studentTable.begin(); itl.hasNext(); ++itl) {
        auto &value = (*itl)->data.value;
        studentCount += value.getSize();
        auto it = value.begin();
        if (it.hasNext()) {
            int year = (*it)->data.getYear();
            min = std::min(year, min);
            max = std::max(year, max);
        }
    }

    for (auto itl = teamTable.begin(); itl.hasNext(); ++itl) {
        auto &value = (*itl)->data.value;
        teamCount += value.getSize();
    }
}
