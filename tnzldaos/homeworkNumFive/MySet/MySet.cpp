#include "MySet.h"
#include <iostream>
using namespace std;

MySet::MySet() {
    size = 0;
    capacity = 4;

    arr = new int[capacity];
}

MySet::MySet(const MySet& mySet) {
    size = 0;
    capacity = 4;
    arr = new int[capacity];

    for(int index = 0 ; index < mySet.length() ; index++) {
        appendElement(mySet.getElement(index));
    }
}

MySet::~MySet() {
    delete[] arr;
}

optional<int> MySet::findInsertIndex(int inputNum) const {
    int arrNum = 0;

    if(size == 0) {
        return 0;
    }
    
    for(int* index = &arr[0] ; index < &arr[size] ; index++) {
        if(inputNum == *index) {
            return nullopt;
        }
        else if(inputNum < *index) {
            return arrNum;
        }
        arrNum++;
    }

    return arrNum;
}

void MySet::insertElement(int insertIndex, int insertNum) {
    if(size + 1 > capacity) {
        resize();
    }

    for(int* index = &arr[size - 1] ; index >= &arr[insertIndex] ; index--) {
        *(index + 1) = *index;
    }

    arr[insertIndex] = insertNum;
    size++;
} 

void MySet::resize() {
    int* newArr = new int[capacity * 2];

    int arrIndex = 0;
    for(int* index = &arr[0] ; index < &arr[size] ; index++) {
        newArr[arrIndex] = *index;
        arrIndex++;
    }

    capacity = capacity * 2;

    delete[] arr;
    arr = newArr;
}

int MySet::length() const {
    return size;
}

int MySet::getElement(int index) const {
    return arr[index];
}

void MySet::appendElement(int element) {
    if(size == capacity) {
        resize();
    }
    arr[size] = element;
    size++;
}

MySet MySet::operator+(const MySet& inputMySet) const {
   MySet resultSet = MySet();
   
   int index1 = 0;
   int index2 = 0;
    while (index1 < size && index2 < inputMySet.length()) {
        int myValue = arr[index1];
        int otherValue = inputMySet.getElement(index2);

        if(myValue > otherValue) {
            resultSet.appendElement(otherValue);
            index2++;
        }
        else if (myValue < otherValue) {
            resultSet.appendElement(myValue);
            index1++;
        }
        else {
            resultSet.appendElement(myValue);
            index1++;
            index2++;
        }
    }

    if(index1 == size && index2 < inputMySet.length()) {
        for(index2 ; index2 < inputMySet.length() ; index2++) {
            resultSet.appendElement(inputMySet.getElement(index2));
        }
    } 
    else if (index2 == inputMySet.length() && index1 < size) {
        for(index1 ; index1 < size ; index1++) {
            resultSet.appendElement(arr[index1]);
        }
    }

    return resultSet;
}

MySet MySet::operator&(const MySet& inputMySet) const {
    MySet resultSet = MySet();
   
   int index1 = 0;
   int index2 = 0;
    while (index1 < size && index2 < inputMySet.length()) {
        int myValue = arr[index1];
        int otherValue = inputMySet.getElement(index2);

        if(myValue > otherValue) {
            index2++;
        }
        else if (myValue < otherValue) {
            index1++;
        }
        else {
            resultSet.appendElement(myValue);
            index1++;
            index2++;
        }
    }

    return resultSet;
}

MySet MySet::operator-(const MySet& inputMySet) const {
    MySet resultSet = MySet();
   
   int index1 = 0;
   int index2 = 0;
    while (index1 < size && index2 < inputMySet.length()) {
        int myValue = arr[index1];
        int otherValue = inputMySet.getElement(index2);

        if(myValue > otherValue) {
            index2++;
        }
        else if (myValue < otherValue) {
            resultSet.appendElement(myValue);
            index1++;
        }
        else {
            index1++;
            index2++;
        }
    }

    if (index2 == inputMySet.length() && index1 < size) {
        for(index1 ; index1 < size ; index1++) {
            resultSet.appendElement(arr[index1]);
        }
    }

    return resultSet;
}

void MySet::operator=(const MySet& mySet) {
    if(this == &mySet) {
        return;
    }
    delete[] arr;

    arr = new int[capacity];

    size = 0;
    for(int index = 0 ; index < mySet.length() ; index++) {
        appendElement(mySet.getElement(index));
    }
}

istream& operator>>(istream& inputStream, MySet& mySet) {
    int num = 0;
    
    inputStream >> num;
    while (num >= 0) {
        optional<int> insertIndex = mySet.findInsertIndex(num);

        if(insertIndex.has_value()) {
            insertIndex == mySet.length() ? mySet.appendElement(num) : mySet.insertElement(insertIndex.value(), num);
        }

        inputStream >> num;
    }

    return inputStream;
}

ostream& operator<<(ostream& outputStream,const MySet& mySet) {
    outputStream << "{";

    for(int index = 0 ; index < mySet.length() ; index++) {
        outputStream << mySet.getElement(index);

        if(index != mySet.length() - 1) {
            outputStream << ", ";
        }
    }
    outputStream << "}";

    return outputStream;
}
