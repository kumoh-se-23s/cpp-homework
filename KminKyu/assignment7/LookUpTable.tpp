#pragma once

template<typename K, typename V>
class LookUpTable;

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable() {
    this->entryArray = new Entry<K, V>[DEFAULT_CAPACITY];
    this->size = 0;
    this->capacity = DEFAULT_CAPACITY;
}

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable(int capacity) {
    this->capacity = capacity;
    this->entryArray = new Entry<K, V>[capacity];
    this->size = 0;
}

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable(const LookUpTable &other) {
    this->capacity = other.capacity;
    this->entryArray = new Entry<K, V>[other.capacity];
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
    delete[] entryArray;
}

template<typename K, typename V>
LookUpTable<K, V>& LookUpTable<K, V>::operator=(const LookUpTable &other) {
    if (this == other) {
        return *this;
    }

    if (this->capacity < other.capacity) {
        this->capacity = other.capacity;
        delete[] entryArray;
        this->entryArray = new Entry<K, V>[other.capacity];
    }

    for (int index = 0; index < other.size; ++index) {
        this->entryArray[index] = other.entryArray[index];
    }
    this->size = other.size;
    return *this;
}

template<typename K, typename V>
LookUpTable<K, V> &LookUpTable<K, V>::operator=(LookUpTable &&other) noexcept {
    if (this == other) {
        return *this;
    }
    delete[] entryArray;

    this->capacity = other.capacity;
    this->entryArray = other.entryArray;
    this->size = other.size;

    other.capacity = 0;
    other.size = 0;
    other.entryArray = nullptr;

    return *this;
}

template<typename K, typename V>
void LookUpTable<K, V>::resize() {
    this->capacity = (this->capacity <= 0) ?  DEFAULT_CAPACITY : this->capacity * 2;
    Entry<K, V> *resultArray = new Entry<K, V>[this->capacity];
    for (int index = 0; index < size; ++index) {
        resultArray[index] = entryArray[index];
    }
    delete[] entryArray;
    this->entryArray = resultArray;
}


template<typename K, typename V>
Entry<K, V> & LookUpTable<K, V>::getEntry(K key) const {
    int index = binarySearch(0, size, key);

    if (index != -1) {
        return entryArray[index];
    } else {
        return Entry(K(), V());
    }
}

template<typename K, typename V>
const V &LookUpTable<K, V>::getValue(K key) const {
    return getEntry(key).value;
}

template<typename K, typename V>
void LookUpTable<K, V>::putIfAbsent(K key, V value) {
    int index = binarySearch(0, size, key);

    if (index != -1) {
        getEntry(key).value = value;
    } else {
        insert(index, key, value);
    }
}

template<typename K, typename V>
void LookUpTable<K, V>::insert(int index, K key, V value) {
    index = std::max(0, index);
    index = std::min(index, size - 1);

    if (this->size + 1 > this->capacity) {
        resize();
    }

    for (int nowIdx = this->size; nowIdx > index; --nowIdx) {
        entryArray[nowIdx] = entryArray[nowIdx - 1];
    }
    entryArray[index] = Entry(key, value);
    ++this->size;
}

template <typename K, typename V>
int LookUpTable<K, V>::binarySearch(int left, int right, K key) const {
    if (left >= right) {
        return std::min(left, right);
    }
    int mid = (left + right) / 2;
    if (entryArray[mid].key < key) {
        return binarySearch(mid + 1, right, key);
    } else if (entryArray[mid].key > key) {
        return binarySearch(left, mid, key);
    } else {
        return mid;
    }
}

