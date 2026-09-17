#include<iostream>
#include"MyArray.h"

using namespace std;

MyArray1::MyArray1() {
    nowSize = 0;
}

bool MyArray1::readAscSorted() {
    nowSize = 0;
    for (int arrSize = 0; arrSize < MAX_ARRAY_SIZE; ++arrSize) {
        int item;
        cin >> item;
        append(item);        
    }
    for (int arrSize = 0; arrSize < nowSize - 1; ++arrSize) {
        if (array[arrSize] > array[arrSize + 1]) {
            return false;
        }   
    }
    return true;
}

void MyArray1::append(int item) {
    if (nowSize != MAX_ARRAY_SIZE) {
        array[nowSize++] = item;
    }
}

void MyArray1::print() const {
    for (int index = 0; index < nowSize; ++index) {
        if (index != 0) {
            cout << " ";
        }
        cout << array[index];
    }
    cout << endl;
}

int MyArray1::getNowSize() const {
    return nowSize;
}
int MyArray1::get(int index) const {
    return array[index];
}
MyArray2 MyArray1::merge(const MyArray1& mergeableArray) const {
    int baseArrayIndex = 0, mergeableArrayIndex = 0;
    MyArray2 mergedArray;
    while (baseArrayIndex < this->nowSize && mergeableArrayIndex < mergeableArray.getNowSize()) {

        int baseArrayValue = this->get(baseArrayIndex), mergeArrayValue = mergeableArray.get(mergeableArrayIndex);
        
        if (baseArrayValue < mergeArrayValue) {
            mergedArray.append(baseArrayValue);
            ++baseArrayIndex;
        } else {
            mergedArray.append(mergeArrayValue);
            ++mergeableArrayIndex;
        }
    }
    while (baseArrayIndex < this->nowSize) {
        mergedArray.append(
        this->get(baseArrayIndex++)
        );

    }
    while (mergeableArrayIndex < mergeableArray.getNowSize()) {
        mergedArray.append(
            mergeableArray.get(mergeableArrayIndex++)
        );
    }
    return mergedArray;
}


MyArray2::MyArray2() {
    nowSize = 0;
}

bool MyArray2::readAscSorted() { //사용하지는 않지만 임시로 구현
    nowSize = 0;
    for (int arrSize = 0; arrSize < MAX_ARRAY_SIZE; ++arrSize) {
        int item;
        cin >> item;
        append(item);
    }
    for (int arrSize = 0; arrSize < nowSize - 1; ++arrSize) {
        if (array[arrSize] > array[arrSize + 1]) {
            return false;
        }   
    }
    return true;
}

void MyArray2::append(int item) {
    if (nowSize != MAX_ARRAY_SIZE) {
        array[nowSize++] = item;
    }
}

void MyArray2::print() const {
    for (int index = 0; index < nowSize; ++index) {
        if (index != 0) {
            cout << " ";
        }
        cout << array[index];
    }
    cout << endl;
}

int MyArray2::getNowSize() const {
    return nowSize;
}
int MyArray2::get(int index) const {
    return array[index];
}

