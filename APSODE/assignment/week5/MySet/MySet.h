//
// Created by apsode on 26. 10. 13..
//

#ifndef CPP_HOMEWORK_MYSET_H
#define CPP_HOMEWORK_MYSET_H
#include "array_list.h"
#include "collection.h"

class MySet : public Collection<int> {
    ArrayList<int> container;

    public:
        ~MySet() override;

        int get(int index) override;

        void set(int index, const int &other) override;

        void add(const int &other) override;

        void add_all(const int *others, int count) override;

        int remove(int index) override;

        int size() override;

        bool empty() override;

        bool contains(const int &object) override;


        MySet operator+(const MySet &mySet) const;

        MySet operator&(const MySet &mySet) const;

        MySet operator-(const MySet &mySet) const;

        MySet& operator=(const MySet &mySet);

    private:
        void require_unique(const int &element);


        MySet unionSet(const MySet &mySet) const;

        MySet intersectionSet(const MySet &mySet) const;

        MySet differenceSet(const MySet &mySet) const;

};

std::istream& operator>>(std::istream& input_stream, MySet& set);

std::ostream& operator<<(std::ostream& input_stream, const MySet& set);

#endif //CPP_HOMEWORK_MYSET_H
