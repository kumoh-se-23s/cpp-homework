#pragma once
#include <iosfwd>

class MySet {
    public:
        MySet();
        MySet(const MySet& set);
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
        int *array = nullptr;
        void resize();
        MySet unionSet(const MySet &set) const;
        MySet intersectionSet(const MySet &set) const;
        MySet differenceSet(const MySet &set) const;

        void append(int val);
        int findPos(int val) const;
        void insertAtPos(int val, int pos);

};

std::ostream& operator <<(std::ostream& os, const MySet& set);
std::istream& operator >>(std::istream& is, MySet& set);

