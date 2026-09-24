#include <iostream>
#include "MyArray.h"

using namespace std;

MyArray2::MyArray2() = default;

void MyArray2::append(int val) {
    if (size < CAPACITY) {
        arr[size++] = val;
    }
}

void MyArray2::print() {
    for (int i = 0; i < CAPACITY; i++) {
        cout << arr[i] << " ";
    }
}

//MyArray1
int MyArray1::getArray(int index) {
    return arr[index];
}

bool MyArray1::readAscSorted() {
    bool isSorted = true;

    for (int i = 0; i < CAPACITY; i++) {
        cin >> arr[i];
        size++;

        if (i > 0 && arr[i - 1] > arr[i]) {
            isSorted = false;
        }
    }
    return isSorted;
}

MyArray2 MyArray1::merge(MyArray1 arr) {
    int i = 0, j = 0;
    MyArray2 resultArr;

    while (i < CAPACITY && j < CAPACITY) {
        if (this->getArray(i) < arr.getArray(j)) {
            resultArr.append(this->getArray(i++));
        } else {
            resultArr.append(arr.getArray(j++));
        }
    }
    while (i < CAPACITY) {
        resultArr.append(this->getArray(i++));
    }
    while (j < CAPACITY) {
        resultArr.append(arr.getArray(j++));
    }

    return resultArr;
}
