//
// Created by KminKyu on 2026-10-12.
//

#include "MySet.h"

MySet::~MySet() {
    delete[] array;
}

MySet::MySet(const MySet &mySet) {
    this->capacity = mySet.capacity;
    this->size = mySet.capacity;
    this->array = new int[mySet.capacity];

    for (int index = 0; index < mySet.size; ++index) {
        array[index] = mySet.array[index];
    }
}

MySet MySet::operator+(const MySet &mySet) const {
    return unionSet(mySet);
}

MySet MySet::operator&(const MySet &mySet) const {
    return intersectionSet(mySet);
}

MySet MySet::operator-(const MySet &mySet) const {
    return differenceSet(mySet);
}

MySet & MySet::operator=(const MySet &mySet) {
    if (this == &mySet) {
        return *this;
    }
    this->capacity = mySet.capacity;
    this->size = mySet.size;

    int *resultArray = new int[mySet.capacity];

    for (int index = 0; index < mySet.size; ++index) {
        resultArray[index] = mySet.array[index];
    }

    delete[] this->array;
    array = resultArray;

    return *this;
}

void MySet::resize(int capacity) {
    this->capacity = capacity;
    int *result = new int[capacity];
    for (int index = 0; index < size; ++index) {
        result[index] = array[index];
    }
    delete[] array;
    array = result;
}

void MySet::clear() {
    size = 0;
}

int MySet::getLength() const {
    return size;
}

int MySet::getCapacity() const {
    return capacity;
}

int MySet::get(int index) const {
    if (0 <= index && index < size) {
        return array[index];
    } else {
        return INT_MIN;
    }
}

MySet MySet::unionSet(const MySet &mySet) const {
    MySet result = MySet();
    int idx1st = 0, idx2nd = 0;
    while (idx1st < this->size && idx2nd < mySet.size) {
        if (array[idx1st] < mySet.array[idx2nd]) {
            result.append(array[idx1st++]);
        } else if (array[idx1st] > mySet.array[idx2nd]) {
            result.append(mySet.array[idx2nd++]);
        } else {
            result.append(array[idx1st++]);
            ++idx2nd;
        }
    }

    while (idx1st < this->size) {
        result.append(array[idx1st++]);
    }

    while (idx2nd < mySet.size) {
        result.append(mySet.array[idx2nd++]);
    }
    return result;

}

MySet MySet::intersectionSet(const MySet &mySet) const {
    MySet result = MySet();
    int idx1st = 0, idx2nd = 0;
    while (idx1st < this->size && idx2nd < mySet.size) {
        if (array[idx1st] == mySet.array[idx2nd]) {
            result.append(array[idx1st++]);
            ++idx2nd;
        } else {
            array[idx1st] < mySet.array[idx2nd] ? ++idx1st : ++idx2nd;
        }
    }

    return result;
}

MySet MySet::differenceSet(const MySet &mySet) const {
    MySet result = MySet();
    int idx1st = 0, idx2nd = 0;
    while (idx1st < this->size && idx2nd < mySet.size) {
        if (array[idx1st] == mySet.array[idx2nd]) {
            ++idx1st;
            ++idx2nd;
        } else if (array[idx1st] < mySet.array[idx2nd]) {
            result.append(array[idx1st++]);
        } else {
            ++idx2nd;
        }
    }

    while (idx1st < this->size) {
        result.append(array[idx1st++]);
    }

    return result;
}

void MySet::append(int appendValue) {
    if (size >= capacity) {
        resize(capacity * 2);
    }
    this->array[size++] = appendValue;
}

void MySet::append(const MySet &mySet) {
    for (int index = 0; index < mySet.size; ++index) {
        if (this->size >= this->capacity) {
            resize(this->capacity * 2);
        }
        array[size++] = mySet.array[index];
    }
}

void MySet::insert(int insertValue) {
    int index = binarySearch(0, this->size, insertValue);

    if (insertValue == array[index]) {
        return;
    }

    if (this->size + 1 > this->capacity) {
        resize(this->capacity * 2);
    }

    for (int nowIdx = this->size++; nowIdx > index; nowIdx--) {
        array[nowIdx] = array[nowIdx - 1];
    }
    array[index] = insertValue;
}

int MySet::binarySearch(int left, int right, int value) {
    if (left >= right) {
        return min(left, right);
    }
    int mid = (left + right) / 2;
    if (array[mid] < value) {
        return binarySearch(mid + 1, right, value);
    } else if (array[mid] > value) {
        return binarySearch(left, mid, value);
    } else {
        return mid;
    }

}

istream & operator>>(istream &in, MySet &mySet) {
    mySet.clear();
    int input;
    do {
        in >> input;
        if (input != -1) {
            mySet.insert(input);
        }
    } while(input != -1);

    return in;
}

ostream & operator<<(ostream &out, const MySet &mySet) {
    out << "{";
    int index;
    for (index = 0; index < mySet.getLength(); ++index) {
        out << mySet.get(index);
        if (index != mySet.getLength() - 1) {
            out << ", ";
        }
    }
    out << "}";
    return out;
}
