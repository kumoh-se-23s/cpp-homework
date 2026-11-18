#pragma once

template<typename K, typename V>
class LookUpTable;

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable() {
    this->entryArray = new Entry<K, LinkedList<V> *>[DEFAULT_CAPACITY];
    this->size = 0;
    this->capacity = DEFAULT_CAPACITY;
}

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable(int capacity) {
    this->capacity = capacity;
    this->entryArray = new Entry<K, LinkedList<V> *>[capacity];
    this->size = 0;
}

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable(const LookUpTable &other) {
    this->capacity = other.capacity;
    this->entryArray = new Entry<K, LinkedList<V> *>[other.capacity];
    for (int index = 0; index < other.size; ++index) {
        this->entryArray[index] = other.entryArray[index];
    }
    this->size = other.size;
}

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable(LookUpTable &&other) noexcept {
    this->capacity = other.capacity;
    this->entryArray = other.entryArray;
    this->size = other.size;

    other.capacity = 0;
    other.size = 0;
    other.entryArray = nullptr;
}

template<typename K, typename V>
LookUpTable<K, V>::~LookUpTable() {
    for (int index = 0; index < size; ++index) {
        delete entryArray[index];
    }
    delete[] entryArray;
}

template<typename K, typename V>
LookUpTable<K, V>& LookUpTable<K, V>::operator=(const LookUpTable &other) {
    if (this == other) {
        return *this;
    }
    for (int index = 0; index < size; ++index) {
        delete this->entryArray[index];
    }

    if (this->capacity < other.capacity) {
        this->capacity = other.capacity;
        delete[] entryArray;
        this->entryArray = new Entry<K, LinkedList<V> *>[other.capacity];
    }

    for (int index = 0; index < other.size; ++index) {
        this->entryArray[index] = new Entry<K, LinkedList<V> *>(other.entryArray[index]);
    }
    this->size = other.size;
    return *this;
}

template<typename K, typename V>
LookUpTable<K, V> & LookUpTable<K, V>::operator=(LookUpTable &&other) noexcept {
}

template<typename K, typename V>
void LookUpTable<K, V>::resize() {
}


template<typename K, typename V>
void LookUpTable<K, V>::setList(const LinkedList<V> &List) {
}