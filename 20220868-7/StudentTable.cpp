//
// Created by Merutilm on 10/28/26.
//

#include "StudentHashTable.h"

#include "StudentCircularLinkedList.h"
#include "Node.h"
#include "TeamMemberCircularLinkedList.h"

StudentHashTable::StudentHashTable() {
}

StudentHashTable::~StudentHashTable() {
    //noop
}

void StudentHashTable::addStudent(Student student) {

    const RAIISharedPointer node(Node(std::move(student)));
    bool inserted = false;

    for (auto it = studentTable.begin(); it.hasNext(); ++it) {
        const int key = (*it)->data.key;
        if (key == student.getYear()) {
            (*it)->data.value.add(node);
            inserted = true;
            break;
        }
        if (key > student.getYear()) {
            KeyValuePair<int, StudentCircularLinkedList> pair;
            pair.key = student.getYear();
            pair.value.add(node);
            auto ptr = RAIISharedPointer(Node(std::move(pair)));
            studentTable.insertBefore(*it, ptr);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        KeyValuePair<int, StudentCircularLinkedList> pair;
        pair.key = student.getYear();
        pair.value.add(node);
        studentTable.add(RAIISharedPointer(Node(std::move(pair))));
    }

    inserted = false;

    for (auto itl = currTeamTable[teamNameHashIdx].begin(); itl.hasNext(); ++itl) {
        if (itl.hasNext() && (*itl)->data.key == student.getTeamId()) {
            inserted = true;
            bool valueFound = false;
            for (auto it = (*itl)->data.value.begin(); it.hasNext(); ++it) {
                if ((*it)->data == student) {
                    valueFound = true;
                }
            }
            if (!valueFound) {
                (*itl)->data.value.add(node);
            }
        }
    }
    if (!inserted) {
        auto nv = TeamMemberCircularLinkedList();
        currTeamTable[teamNameHashIdx].add(RAIISharedPointer(Node(KeyValuePair{
            .key = student.getTeamId(),
            .value = std::move(nv)
        })));
        ++teamTableWeight;
    }
}

void StudentHashTable::removeStudent(const Student &student) const {
    const int yearHash = student.makeYearHash();
    const int teamHash = student.makeTeamHash();
    const int studentHashIdx = hash(yearHash, studentNumHashTableCapacity);
    const int teamNameHashIdx = hash(teamHash, teamNumHashTableCapacity);


    for (auto itl = studentTable[studentHashIdx].begin(); itl.hasNext(); ++itl) {
        KeyValuePair<int, StudentCircularLinkedList> &entry = (*itl)->data;
        if (entry.value.getSize() > 0 && entry.key == student.getYear()) {
            entry.value.remove(student);
        }
    }

    for (auto itl = teamTable[teamNameHashIdx].begin(); itl.hasNext(); ++itl) {
        KeyValuePair<int, TeamMemberCircularLinkedList> &entry = (*itl)->data;
        if (entry.value.getSize() > 0 && entry.key == student.getTeamId()) {
            entry.value.remove(student);
        }
    }
}

MyString StudentHashTable::toString() const {
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

    for (int i = 0; i < studentNumHashTableCapacity; ++i) {
        for (auto itl = studentTable[i].begin(); itl.hasNext(); ++itl) {
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

                    const int teamHash = hash(student.makeTeamHash(), teamNumHashTableCapacity);
                    for (auto teamItl = teamTable[teamHash].begin(); teamItl.hasNext(); ++teamItl) {
                        bool delim = false;
                        if ((*teamItl)->data.key == teamId) {
                            for (auto teamIt = (*teamItl)->data.value.begin(); teamIt.hasNext(); ++teamIt) {
                                if ((*teamIt)->data != student) {
                                    if (delim) {
                                        result.append(" - ");
                                    } else {
                                        result.append(" ");
                                    }
                                    result.append((*teamIt)->data.toString());
                                    delim = true;
                                }
                            }
                        }
                    }
                    result.append(")");
                }
                result.append('\n');
            }
        }
    }
    return result;
}

void StudentHashTable::getStudentInfos(int &min, int &max, int &studentCount, int &teamCount) const {
    min = INT_MAX;
    max = INT_MIN;
    studentCount = 0;
    teamCount = 0;

    for (int i = 0; i < studentNumHashTableCapacity; ++i) {
        for (auto itl = studentTable[i].begin(); itl.hasNext(); ++itl) {
            auto &value = (*itl)->data.value;
            studentCount += value.getSize();
            auto it = value.begin();
            if (it.hasNext()) {
                int year = (*it)->data.getYear();
                min = std::min(year, min);
                max = std::max(year, max);
            }
        }
    }
    for (int i = 0; i < teamNumHashTableCapacity; ++i) {
        for (auto itl = teamTable[i].begin(); itl.hasNext(); ++itl) {
            auto &value = (*itl)->data.value;
            teamCount += value.getSize();
        }
    }
}

