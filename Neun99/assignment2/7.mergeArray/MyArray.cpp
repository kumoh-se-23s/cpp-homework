#include <iostream>
#include "MyArray.h"

using namespace std;

//MyArray1-----------
int MyArray1::length() {
    return LENGTH;
}

int MyArray1::size() {
    return usingSize;
}

void MyArray1::append(int value) {
    arr[usingSize] = value;
    usingSize++;
}

int MyArray1::get(int idx) {
    return arr[idx];
}

bool MyArray1::readAscSorted() {
    for (int idx = 0; idx < LENGTH; idx++) {
        int input;
        cin >> input;
        append(input);
    }
    for (int idx = 0; idx < LENGTH - 1; idx++) {
        if (arr[idx] > arr[idx+1])
            return false;
    }
    return true;
}

MyArray2 MyArray1::merge(MyArray1 arr2) {
    MyArray2 resultArr;

    int arr1Idx = 0;
    int arr2Idx = 0;

    while (arr1Idx < size() && arr2Idx < arr2.size()) {
        if (arr[arr1Idx] >= arr2.get(arr2Idx)) {
            resultArr.append(arr2.get(arr2Idx));
            arr2Idx++;
        } else {
            resultArr.append(get(arr1Idx));
            arr1Idx++;
        }
    }

    while (arr1Idx < size()) {
        resultArr.append(get(arr1Idx));
        arr1Idx++;
    }

    while (arr2Idx < arr2.size()) {
        resultArr.append(arr2.get(arr2Idx));
        arr2Idx++;
    }

    return resultArr;
}

//MyArray2---------------
int MyArray2::length() {
    return LENGTH;
}

int MyArray2::size() {
    return usingSize;
}

void MyArray2::append(int value) {
    arr[usingSize] = value;
    usingSize++;
}

int MyArray2::get(int idx) {
    return arr[idx];
}

void MyArray2::print() {
    cout << get(0);
    for (int idx = 1; idx < size(); idx++) {
        cout << " " << get(idx);
    }
    cout << endl;
}