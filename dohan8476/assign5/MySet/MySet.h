#pragma once
#include <iosfwd>

class MySet {
    public:
        MySet();
        ~MySet();

        int getSize() const;
        int getArray(int index) const;

        void insert(int value);

        MySet operator+(const MySet& set) const;
        MySet operator-(const MySet& set) const;
        MySet operator&(const MySet& set) const;
        MySet& operator=(const MySet& set);

    private:
        int size = 0;
        int capacity = 4;
        int *array = new int[capacity];
        void resize();
        MySet unionSet(const MySet &set) const;
        MySet intersectionSet(const MySet &set) const;
        MySet differenceSet(const MySet &set) const;

        void append(const MySet &set);
};

std::ostream& operator <<(std::ostream& out, const MySet& set);
std::istream& operator >>(std::istream& in, MySet& set);

