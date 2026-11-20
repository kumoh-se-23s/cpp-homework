#pragma once

template<typename T>
void MyArrayList<T>::resize() {
    int newCapacity = capacity * 2;
    T *newArrayList = new T[newCapacity];

    for (int index = 0; index < size; index++) {
        newArrayList[index] = arrayList[index];
    }

    delete[] arrayList;
    arrayList = newArrayList;
    capacity = newCapacity;
}

template<typename T>
MyArrayList<T>::~MyArrayList() {
    delete[] arrayList;
}

template<typename T>
void MyArrayList<T>::append(const T &element) {
    if (size == capacity) {
        resize();
    }

    arrayList[size] = element;
    size++;
}

template<typename T>
int MyArrayList<T>::getSize() const { return size; }

template<typename T>
T &MyArrayList<T>::get(int index) { return arrayList[index]; }

template<typename T>
void MyArrayList<T>::remove(const string &input) {
    for (int index = 0; index < size; index++) {
        if (arrayList[index].getKey() == input) {
            for (int moveIndex = index; moveIndex < size - 1; moveIndex++) {
                arrayList[moveIndex] = arrayList[moveIndex + 1];
            }
            size--;
            return;
        }
    }
}

template<typename T>
T *MyArrayList<T>::findObject(const string &input) {
    for (int index = 0; index < size; index++) {
        T &element = arrayList[index];
        const string &key = element.getKey();

        if (key == input) {
            return &element;
        }
    }
    return nullptr;
}
