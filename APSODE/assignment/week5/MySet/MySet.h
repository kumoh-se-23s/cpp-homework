//
// Created by apsode on 26. 10. 13..
//

#ifndef CPP_HOMEWORK_MYSET_H
#define CPP_HOMEWORK_MYSET_H
#include <istream>
#include <ostream>

#include "array_list.h"
#include "collection.h"

class MySet : public Collection<int> {
    ArrayList<int> container;

    public:
        MySet();

        MySet(const MySet &other);

        ~MySet() override;


        int get(int index) const override;

        void set(int index, const int &other) override;

        void add(const int &other) override;

        void add_all(const int *others, int count) override;

        int remove(int index) override;

        int size() const override;

        bool empty() const override;

        bool contains(const int &object) const override;


        MySet operator+(const MySet &mySet) const;

        MySet operator&(const MySet &mySet) const;

        MySet operator-(const MySet &mySet) const;

        MySet& operator=(const MySet &mySet);

    private:
        void require_unique(const int &element) const;


        MySet unionSet(const MySet &mySet) const;

        MySet intersectionSet(const MySet &mySet) const;

        MySet differenceSet(const MySet &mySet) const;

        int binary_search(int target) const;

        int binary_search(int target, int low, int high) const;

};

std::istream& operator>>(std::istream& input_stream, MySet& set);

std::ostream& operator<<(std::ostream& output_stream, const MySet& set);

#endif //CPP_HOMEWORK_MYSET_H
