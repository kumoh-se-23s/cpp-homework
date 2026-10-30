#include "MySet.h"

MySet::~MySet() {
    delete[] array;
}

MySet::MySet(const MySet &otherSet) {
    this->array = new int[otherSet.capacity];
    this->capacity = otherSet.capacity;
    this->size = otherSet.size;


    for (int index = 0; index < otherSet.size; ++index) {
        array[index] = otherSet.array[index];
    }
}
MySet::MySet(MySet &&otherSet) noexcept {
    this->size = otherSet.size;
    this->capacity = otherSet.capacity;
    this->array = otherSet.array;

    otherSet.array = nullptr;
    otherSet.size = 0;
    otherSet.capacity = 0;
}

MySet& MySet::operator=(const MySet &otherSet) {
    if (this == &otherSet) {
        return *this;
    }
    int *resultArray = new int[otherSet.capacity];
    this->capacity = otherSet.capacity;
    this->size = otherSet.size;



    for (int index = 0; index < otherSet.size; ++index) {
        resultArray[index] = otherSet.array[index];
    }

    delete[] this->array;
    array = resultArray;

    return *this;
}

MySet& MySet::operator=(MySet &&otherSet) noexcept {
    if (this == &otherSet) {
        return *this;
    }
    delete[] this->array;

    this->size = otherSet.size;
    this->capacity = otherSet.capacity;
    this->array = otherSet.array;

    otherSet.capacity = 0;
    otherSet.size = 0;
    otherSet.array = nullptr;

    return *this;

}

MySet MySet::operator+(const MySet &otherSet) const {
    return unionSet(otherSet);
}

MySet MySet::operator&(const MySet &otherSet) const {
    return intersectionSet(otherSet);
}

MySet MySet::operator-(const MySet &otherSet) const {
    return differenceSet(otherSet);
}

int MySet::getLength() const {
    return size;
}

int MySet::getCapacity() const {
    return capacity;
}

void MySet::clear() {
    size = 0;
}

void MySet::resize() {
    this->capacity = (capacity == 0) ? 4 : capacity * 2;
    int *result = new int[capacity];
    for (int index = 0; index < size; ++index) {
        result[index] = array[index];
    }
    delete[] array;
    array = result;
}

int MySet::get(int index) const {
    if (0 <= index && index < size) {
        return array[index];
    } else {
        return INT_MIN;
    }
}

MySet MySet::unionSet(const MySet &otherSet) const {
    MySet result = MySet();
    int idx1st = 0, idx2nd = 0;
    while (idx1st < this->size && idx2nd < otherSet.size) {
        if (array[idx1st] < otherSet.array[idx2nd]) {
            result.append(array[idx1st++]);
        } else if (array[idx1st] > otherSet.array[idx2nd]) {
            result.append(otherSet.array[idx2nd++]);
        } else {
            result.append(array[idx1st++]);
            ++idx2nd;
        }
    }

    while (idx1st < this->size) {
        result.append(array[idx1st++]);
    }

    while (idx2nd < otherSet.size) {
        result.append(otherSet.array[idx2nd++]);
    }
    return result;

}

MySet MySet::intersectionSet(const MySet &otherSet) const {
    MySet result = MySet();
    int idx1st = 0, idx2nd = 0;
    while (idx1st < this->size && idx2nd < otherSet.size) {
        if (array[idx1st] == otherSet.array[idx2nd]) {
            result.append(array[idx1st++]);
            ++idx2nd;
        } else {
            array[idx1st] < otherSet.array[idx2nd] ? ++idx1st : ++idx2nd;
        }
    }

    return result;
}

MySet MySet::differenceSet(const MySet &otherSet) const {
    MySet result = MySet();
    int idx1st = 0, idx2nd = 0;
    while (idx1st < this->size && idx2nd < otherSet.size) {
        if (array[idx1st] == otherSet.array[idx2nd]) {
            ++idx1st;
            ++idx2nd;
        } else if (array[idx1st] < otherSet.array[idx2nd]) {
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
        resize();
    }
    this->array[size++] = appendValue;
}

void MySet::append(const MySet &otherSet) {
    for (int index = 0; index < otherSet.size; ++index) {
        if (this->size >= this->capacity) {
            resize();
        }
        array[size++] = otherSet.array[index];
    }
}

void MySet::insert(int insertValue) {
    int index = binarySearch(0, this->size, insertValue);

    if (index == -1) {
        return;
    }

    if (this->size + 1 > this->capacity) {
        resize();
    }

    for (int nowIdx = this->size; nowIdx > index; nowIdx--) {
        array[nowIdx] = array[nowIdx - 1];
    }
    array[index] = insertValue;
    ++this->size;
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
        return -1;
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
