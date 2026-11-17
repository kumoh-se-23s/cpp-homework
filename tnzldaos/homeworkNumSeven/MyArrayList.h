//
// Created by 김지환 on 26. 11. 17..
//

#ifndef CPP_HOMEWORK_MYARRAYLIST_H
#define CPP_HOMEWORK_MYARRAYLIST_H

template<typename T>
class MyArrayList {
    int capacity{8};
    int size{0};

    T *array;

    void resize() {
        capacity = capacity * 2;
        T *newArray = new T[capacity];
        size = 0;

        for (int index = 0; index < size; index++) {
            newArray[index] = array[index];
            size++;
        }

        delete[] array;
        array = newArray;
    }

public:
    MyArrayList() : array(new T[capacity]) {
    }

    int length() const { return size; }

    void append(const T &element) {
        if (size > capacity - 1) {
            resize();
        }

        array[size] = element;
        size++;
    }

    T getElement(int index) const {
        return array[index];
    }
};


#endif //CPP_HOMEWORK_MYARRAYLIST_H
