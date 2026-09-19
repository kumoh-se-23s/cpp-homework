#pragma once

class MyArray2 {
    public:
        MyArray2() = default;
        ~MyArray2() = default;

        void print() const;

        int &operator[](int index);
        const int &operator[](int index) const;

    private:
        static constexpr int DEFAULT_ARRAY_CAPACITY = 10;

        int array[DEFAULT_ARRAY_CAPACITY] = {};
};

class MyArray1 {
    public:
        MyArray1() = default;
        ~MyArray1() = default;

        bool is_asc_sorted() const;

        bool readAscSorted();

        int &operator[](int index);
        const int &operator[](int index) const;

        MyArray2 merge(MyArray1 other_array) const;
        
    private:
        static constexpr int DEFAULT_ARRAY_CAPACITY = 5;
        int array[DEFAULT_ARRAY_CAPACITY] = {};
};
