#include <iostream>
#include "MyArray.h"

using namespace std;

MyArray2::MyArray2() {
    int array[SIZE] = {};
}

void MyArray2::setArray(int val, int index) {
    arr[index] = val;
}

int MyArray1::getArray(int index) {
    return arr[index];
}

bool MyArray1::readAscSorted() {
    for (int i = 0; i < SIZE; ++i) {
        cin >> arr[i];
    }

    for (int i = 0; i < SIZE - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

MyArray2 MyArray1::merge(MyArray1 arr) {
    int idx = 0, i = 0, j = 0;
    MyArray2 resultArr;

    while (i < SIZE && j < SIZE) {
        if (this->getArray(i) < arr.getArray(j)) {
            resultArr.setArray(this->getArray(i++), idx++);
        } else {
            resultArr.setArray(arr.getArray(j++), idx++);
        }
    }
    while (i < SIZE) {
        resultArr.setArray(this->getArray(i++), idx++);
    }
    while (j < SIZE) {
        resultArr.setArray(arr.getArray(j++), idx++);
    }

    return resultArr;
}

void MyArray2::print() {
    for (int i = 0; i < SIZE; i++) {
        cout << arr[i] << " ";
    }
}
