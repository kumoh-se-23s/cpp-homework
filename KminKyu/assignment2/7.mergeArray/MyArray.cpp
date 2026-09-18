#include<iostream>
#include"MyArray.h"


MyArray1::MyArray1() {
    nowSize = 0;
}

bool MyArray1::readAscSorted() {
    nowSize = 0;
    bool flag = true;
    for (int arrSize = 0; arrSize < MAX_ARRAY_SIZE; ++arrSize) {
        int item;
        std::cin >> item;
        array[arrSize] = item;
        ++nowSize;
        if (arrSize != 0 && array[arrSize - 1] > array[arrSize]) {
            flag = false;
        }
    }
    return flag;
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
    while (baseArrayIndex < this->getNowSize() && mergeableArrayIndex < mergeableArray.getNowSize()) {

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

void MyArray2::append(int item) {
    if (nowSize != MAX_ARRAY_SIZE) {
        array[nowSize++] = item;
    }
}

void MyArray2::print() const {
    for (int index = 0; index < nowSize; ++index) {
        if (index != 0) {
            std::cout << " ";
        }
        std::cout << array[index];
    }
    std::cout << "\n";
}

int MyArray2::getNowSize() const {
    return nowSize;
}
int MyArray2::get(int index) const {
    return array[index];
}

