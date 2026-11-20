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
    if (capacity <=0) {
        LookUpTable();
    } else {
        this->capacity = capacity;
        this->entryArray = new Entry<K, V>[capacity];
        this->size = 0;
    }
}

template<typename K, typename V>
LookUpTable<K, V>::LookUpTable(const LookUpTable &other) {
    this->capacity = other.capacity;
    this->entryArray = new Entry<K, V>[other.capacity];
    for (int index = 0; index < other.size; ++index) {
        this->entryArray[index] = std::move(other.entryArray[index]);
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
    if (this == &other) {
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
    if (this == &other) {
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
        resultArray[index] = std::move(entryArray[index]);
    }
    delete[] entryArray;
    this->entryArray = resultArray;
}


template<typename K, typename V>
Entry<K, V> *LookUpTable<K, V>::findEntry(const K& key){
    int index = binarySearch(0, size, key);

    if (index >= size || entryArray[index].getKey() != key) {
        return nullptr;
    }
    return entryArray + index;
}


template<typename K, typename V>
const Entry<K, V>* LookUpTable<K, V>::findEntry(const K& key) const {
    int index = binarySearch(0, size, key);
    if (index >= size || entryArray[index].getKey() != key) {
        return nullptr;
    }
    return entryArray + index;
}

template<typename K, typename V>
V *LookUpTable<K, V>::getValue(const K& key) const {
    const Entry<K, V>* entry = findEntry(key);
    if (entry == nullptr) {
        return nullptr;
    }
    return entry->getValue();
}

template<typename K, typename V>
const Entry<K, V>* LookUpTable<K, V>::at(int index) const {
    if (index < 0 || index >= size) {
        return nullptr;
    }
    return entryArray + index;
}

template<typename K, typename V>
void LookUpTable<K, V>::removeByKey(const K &key) {
    int pos = binarySearch(0, size, key);
    for (int index = pos;  index < size - 1; ++index) {
        entryArray[index] = std::move(entryArray[index + 1]);
    }
    --size;
}


template<typename K, typename V>
void LookUpTable<K, V>::insert(int index, const K& key, V *value) {
    if (this->size + 1 > this->capacity) {
        resize();
    }

    for (int nowIdx = this->size; nowIdx > index; --nowIdx) {
        entryArray[nowIdx] = std::move(entryArray[nowIdx - 1]);
    }
    entryArray[index] = Entry(key, value);
    ++this->size;
}

template<typename K, typename V>
void LookUpTable<K, V>::insert(const K& key, V *value) {
    int index = binarySearch(0, size, key);
    insert(index, key, value);
}

template<typename K, typename V>
int LookUpTable<K, V>::getSize() const {
    return size;
}

template<typename K, typename V>
int LookUpTable<K, V>::getCapacity() const {
    return capacity;
}

template <typename K, typename V>
int LookUpTable<K, V>::binarySearch(int left, int right, const K& key) const {
    if (left >= right) {
        return std::min(left, right);
    }
    int mid = (left + right) / 2;
    if (entryArray[mid].getKey() < key) {
        return binarySearch(mid + 1, right, key);
    } else if (entryArray[mid].getKey()> key) {
        return binarySearch(left, mid, key);
    } else {
        return mid;
    }
}

template<typename K, typename V>
MyString LookUpTable<K, V>::toString() const {
    MyString result = MyString();
    for (int index = 0; index < size; ++index) {
        result += entryArray[index].toString();
    }
    return result;
}

template<typename K, typename V>
bool LookUpTable<K, V>::isEntry(const K &key) {
    if (findEntry(key) != nullptr) {
        return true;
    }
    return false;
}

template<typename K, typename V>
std::ostream & operator<<(std::ostream &out, const LookUpTable<K, V> &lookUpTable) {
    for (int index = 0; index < lookUpTable.getSize(); ++index) {
        if (lookUpTable.at(index) != nullptr) {
            out << *lookUpTable.at(index);
        }

    }
    return out;
}

