#pragma once

class MySet {
    public:
        MySet();
        ~MySet();

    private:
        int size = 0;
        int capacity = 4;
        void resize();

        void unionSet();
        void intersectionSet();
        void differenceSet();

        void insertionSort();

};

