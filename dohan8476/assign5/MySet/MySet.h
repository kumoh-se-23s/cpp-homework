#pragma once
#include <iosfwd>

class MySet {
    public:
        MySet();
        ~MySet();

        MySet operator+(const MySet& set) const;
        MySet operator-(const MySet& set) const;
        MySet operator&(const MySet& set) const;
        MySet& operator=(const MySet& set);

    private:
        int *array = new int[capacity];
        int size = 0;
        int capacity = 4;
        void resize();

        MySet unionSet(const MySet &set) const;

        MySet intersectionSet(const MySet &set) const;

        MySet differenceSet(const MySet &set) const;

        void insert(int value);

};

std::ostream& operator <<(std::ostream& out, const MySet& set);
std::istream& operator >>(std::istream& in, MySet& set);

