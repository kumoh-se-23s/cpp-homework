//
// Created by Merutilm on 10/28/26.
//

#pragma once
#include "Student.h"
#include <stop_token>

#include "DefaultHasher.h"
#include "GenericCircularLinkedList.h"
#include "HashMap.h"
#include "KeyValuePair.h"
#include "../20220868-8/StringHasher.h"
#include "StudentSingleLinkedList.h"
#include "TeamMemberCircularLinkedList.h"

class StudentTable {
    GenericCircularLinkedList<KeyValuePair<int, StudentSingleLinkedList>> studentList;
    HashMap<int, RAIISharedPointer<DoubleNode<KeyValuePair<int, StudentSingleLinkedList>>>, DefaultHasher<int>> studentHashMap;
    GenericCircularLinkedList<KeyValuePair<MyString, TeamMemberCircularLinkedList>> teamList;
    HashMap<MyString, RAIISharedPointer<DoubleNode<KeyValuePair<MyString, TeamMemberCircularLinkedList>>>, StringHasher> teamHashMap;

    constexpr static int INITIAL_CAPACITY = 16;

public:
    explicit StudentTable();

    ~StudentTable();

    StudentTable(const StudentTable &) = delete;

    StudentTable &operator=(const StudentTable &) = delete;

    StudentTable(StudentTable &&) = delete;

    StudentTable &operator=(StudentTable &&) = delete;

    void addStudent(Student student);

    void removeStudent(int num);

    [[nodiscard]] MyString toString() const;

    void exportFile(const MyString & filename) const;

    void getStudentInfos(int &min, int &max, int &studentCount, int &teamCount) const;

};


inline std::ostream &operator<<(std::ostream &os, const StudentTable &table) {
    return os << table.toString();
}
