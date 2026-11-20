#pragma once

#include "SinglyLinkedList.h"
#include "StudentNode.h"

class YearHashTable {
    static constexpr int EMPTY_MAX_YEAR = -1;
    static constexpr int EMPTY_MIN_YEAR = 9999;

    SinglyLinkedList<StudentNode>* buckets;
    int baseYear;
    int minYear;
    int maxYear;
    int capacity;

    public:
        YearHashTable();
        ~YearHashTable();

        void init(int minY, int maxY);
        void insert(StudentNode* node);
        StudentNode* remove(int studentID);
        void clearAndDeleteAllNodes() const;

        int getMinYear() const;
        int getMaxYear() const;
        //수정용
        SinglyLinkedList<StudentNode>& getBucketByYear(int year);
        //읽기용
        const SinglyLinkedList<StudentNode>& getBucketByYear(int year) const;

        bool isExist(int studentID) const;

    private:
        int hash(int year) const;
        void resize(int newYear);
        void updateBounds();
};