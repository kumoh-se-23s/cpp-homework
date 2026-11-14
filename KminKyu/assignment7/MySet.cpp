#include "MySet.h"

template <typename T>
MySet<T>::~MySet() {
    for (int index = 0; index < size; ++index) {
        delete array[index];
    }
    delete[] array;
}

template <typename T>
MySet<T>::MySet(const MySet<T> &otherSet) {
    this->array = new T[otherSet.capacity];
    this->capacity = otherSet.capacity;
    this->size = otherSet.size;


    for (int index = 0; index < otherSet.size; ++index) {
        array[index] = otherSet.array[index];
    }
}

template <typename T>
MySet<T>::MySet(MySet<T> &&otherSet) noexcept {
    this->size = otherSet.size;
    this->capacity = otherSet.capacity;
    this->array = otherSet.array;

    otherSet.array = nullptr;
    otherSet.size = 0;
    otherSet.capacity = 0;
}

template <typename T>
MySet<T>& MySet<T>::operator=(const MySet<T> &otherSet) {
    if (this == &otherSet) {
        return *this;
    }
    T *resultArray;
    if (otherSet.capacity >= this->capacity) {
        resultArray = this->array;
    } else {
        resultArray = new T[otherSet.capacity];
    }

    for (int index = 0; index < otherSet.size; ++index) {
        resultArray[index] = otherSet.array[index];
    }

    if (this->array != resultArray) {
        for (int index = 0; index < size; ++index) {
            delete this->array[index];
        }
        delete[] this->array;
        array = resultArray;
         this->capacity = otherSet.capacity;
    }

    this->size = otherSet.size;

    return *this;
}

template <typename T>
MySet<T>& MySet<T>::operator=(MySet<T> &&otherSet) noexcept {
    if (this == &otherSet) {
        return *this;
    }

    for (int index = 0; index < size; ++index) {
        delete this->array[index];
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

template <typename T>
MySet<T> MySet<T>::operator+(const MySet<T> &otherSet) const {
    return unionSet(otherSet);
}

template <typename T>
int MySet<T>::getLength() const {
    return size;
}

template <typename T>
int MySet<T>::getCapacity() const {
    return capacity;
}

template <typename T>
void MySet<T>::clear() {
    size = 0;
}

template <typename T>
void MySet<T>::resize() {
    this->capacity = (capacity == 0) ? resize(DEFAULT_CAPACITY) : resize(capacity * 2);
}

template <typename T>
int MySet<T>::calcInitialCapacity(int arraySize) {
    if (arraySize <= DEFAULT_CAPACITY) {
        return DEFAULT_CAPACITY;
    }
    int result;
    for (result = DEFAULT_CAPACITY; result <= arraySize; result *= 2) {}
    return result;
}

template <typename T>
void MySet<T>::resize(int capacity) {
    int *result = new T[capacity];
    for (int index = 0; index < size; ++index) {
        result[index] = array[index];
    }
    for (int index = 0; index < size; ++index) {
        delete this->array[index];
    }
    delete[] this->array;
    array = result;
}

template <typename T>
T MySet<T>::get(int index) const {
    if (0 <= index && index < size) {
        return array[index];
    } else {
        return T();
    }
}

template <typename T>
MySet<T> MySet<T>::unionSet(const MySet &otherSet) const {
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

template <typename T>
void MySet<T>::append(const T& appendValue) {
    if (size >= capacity) {
        resize();
    }
    this->array[size++] = appendValue;
}

template <typename T>
void MySet<T>::append(const MySet<T> &otherSet) {
    if (this-> capacity <= this->size + otherSet.size) {
        resize(calcInitialCapacity(this->size + otherSet.size));
    }
    for (int index = 0; index < otherSet.size; ++index) {
        if (this->size >= this->capacity) {
            resize();
        }
        array[size++] = otherSet.array[index];
    }
}

template <typename T>
void MySet<T>::insert(const T& insertValue) {
    int index = binarySearch(0, this->size, insertValue);

    if ()

    if (this->size + 1 > this->capacity) {
        resize();
    }

    for (int nowIdx = this->size; nowIdx > index; nowIdx--) {
        array[nowIdx] = array[nowIdx - 1];
    }
    array[index] = insertValue;
    ++this->size;
}

template <typename T>
int MySet<T>::binarySearch(int left, int right, const T& value) const {
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
