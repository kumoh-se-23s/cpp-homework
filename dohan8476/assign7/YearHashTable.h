#pragma once

#include "SinglyLinkedList.h"
#include "StudentNode.h"

class YearHashTable {
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
        SinglyLinkedList<StudentNode>& getBucketByYear(int year);
        const SinglyLinkedList<StudentNode>& getBucketByYear(int year) const;

    private:
        int hash(int year) const;
        void resize(int newYear);
        void updateBounds();
};