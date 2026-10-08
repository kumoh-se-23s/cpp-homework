#pragma once
#include <iosfwd>

class MySet {
    public:
        MySet();
        ~MySet();

        const MySet& operator+(const MySet& set) const;
        const MySet& operator-(const MySet& set) const;
        const MySet& operator*(const MySet& set) const;

        MySet& operator=(const MySet& set);

    private:
        int *array = new int[capacity];
        int size = 0;
        int capacity = 4;
        void resize();

        void unionSet();
        void intersectionSet();
        void differenceSet();

        void insertionSort(const int *array, int size);

        void swap(int &a,int &b) noexcept;
        void swap(int a, int b);
};

std::ostream& operator <<(std::ostream& out, const MySet& set);
std::istream& operator >>(std::istream& in, MySet& set);

