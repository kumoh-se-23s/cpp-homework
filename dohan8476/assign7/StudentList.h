// #pragma once
//
// #include "TeamHashTable.h"
// #include "YearHashTable.h"
//
// class StudentList {
//     YearHashTable yearTable;
//     TeamHashTable teamTable;
//     int studentCount;
//
//     public:
//         StudentList();
//         ~StudentList();
//
//         void initYearRange(int minYear, int maxYear);
//         void addStudent(const Student& student);
//         bool deleteStudent(int studentID);
//         void printList(std::ostream& os) const;
//
//         int getMinYear() const;
//         int getMaxYear() const;
//         int getStudentCount() const;
//         int getGroupCount() const;
//         const YearHashTable& getYearTable() const;
// };
//
// std::ostream& operator<<(std::ostream& os, const StudentList& list);