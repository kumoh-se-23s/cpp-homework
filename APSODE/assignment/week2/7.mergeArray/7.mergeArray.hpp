#pragma once

class MyArray2 {
    private:
        static const int DEFAULT_ARRAY_CAPACITY = 5;
        int array[DEFAULT_ARRAY_CAPACITY] = {};
        int size = 0;

    public:
        MyArray() {}
        ~MyArray() {}

        void print();

};

class MyArray1 {
    private:
        static const int DEFAULT_ARRAY_CAPACITY = 5;
        int array[DEFAULT_ARRAY_CAPACITY] = {};
        int size = 0;

    public:
        MyArray() {};
        ~MyArray() {};
        
        bool is_asc_sorted();
        bool is_dsc_sorted();

        void get();


        bool readAscSorted();
        MyArray2 merge(MyArray1 array);
        void print();


};
