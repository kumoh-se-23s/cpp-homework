#include "MySet.h"
#include <iomanip>

// =================================================================
// PUBLIC SECTION
// =================================================================

MySet::MySet() = default;

MySet::MySet(const MySet &set) {
    this->capacity = set.capacity;
    this->size = set.size;

    this->array = new int[this->capacity];
    for (int i = 0 ; i < this->size ; i++) {
        this->array[i] = set.array[i];
    }
}


MySet::~MySet() {
    delete [] array;
    array = nullptr;
}


void MySet::insert(const int value) {
    bool isExist = false;
    int pos = 0;

    for (pos = 0; pos < this->size; pos++) {
        //중복 검사
        if (this->array[pos] == value) {
            isExist = true;
            break;
        }
        //위치 검사
        if (this->array[pos] > value) {
            break;
        }
    }

    if (!isExist) {
        //값 삽입 전 size 검사
        if (this->size >= this->capacity) {
            this->resize();
        }
        // 정렬 된 덩어리 뒤로 Shift
        for (int k = this->size; k > pos; k--) {
            this->array[k] = this->array[k - 1];
        }
        this->array[pos] = value;
        this->size++;
    }
}

int MySet::getSize() const {
    return this->size;
}

int MySet::getArray(const int index) const {
    if (0 <= index && index < this->size) {
        return this->array[index];
    }
    return -1;
}

// =================================================================
// OPERATOR OVERLOADING SECTION
// =================================================================

MySet MySet::operator+(const MySet& set) const{
    return this->unionSet(set);
}

MySet MySet::operator-(const MySet& set) const{
    return this->differenceSet(set);
}

MySet MySet::operator&(const MySet& set) const{
    return this->intersectionSet(set);
}

std::istream& operator>>(std::istream& is, MySet& set) {
    int value;

    while (is >> value) {
        if (value < 0) {
            break;
        }
        set.insert(value);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const MySet& set) {
    os << "{" ;
    if (set.getSize() > 0) {
        os << set.getArray(0);
        for (int i = 1; i < set.getSize(); i++) {
            os << ", " << set.getArray(i);
        }
    }
    os << "}";

    return os;
}

MySet& MySet::operator=(const MySet& set) {
    if (this == &set) {
        return *this;
    }
    //기존 동적 배열 반드시 delete
    delete [] array;
    //크기 복사 후 새로 할당
    capacity = set.capacity;
    size = set.size;
    array = new int[capacity];

    for (int i = 0 ; i < set.size ; i++) {
        array[i] = set.array[i];
    }
    return *this;
}

// =================================================================
// PRIVATE SECTION
// =================================================================

void MySet::resize() {
    if (size >= capacity) {
        capacity *= 2;
    }
    //const auto newArray = new int[capacity]; 이거랑 같다는데 머지
    //auto << 옆의 값보고 type 추론해서 하는 놈?
    int *newArray = new int[capacity];

    for (int i = 0 ; i < size ; i++) {
        newArray[i] = array[i];
    }
    delete [] array;
    array = newArray;
}

MySet MySet::unionSet(const MySet &set) const{
    MySet result;
    int i = 0; int j = 0;

    //A, B를 정렬으로 받음 >> merge로 Union 구현
    while (i < this->size && j < set.size) {
        if (this->array[i] < set.array[j]) {
            //A append
            result.append(this->array[i++]);
        } else if (this->array[i] > set.array[j]) {
            //B append
            result.append(set.array[j++]);
        }
        //A교집합B 넣어야함
        else {
            result.append(this->array[i]);
            i++;
            j++;
        }
    }
    //나머지 append
    while (i < this->size) {
        result.append(this->array[i++]);
    }
    while (j < set.size) {
        result.append(set.array[j++]);
    }

    return result;
}

MySet MySet::intersectionSet(const MySet &set) const{
    MySet result;
    int i = 0; int j = 0;

    while (i < this->size && j < set.size) {
        if (this->array[i] < set.array[j]) {
            //A 넣으면 안됨
            i++;
        } else if (this->array[i] > set.array[j]) {
            //B 넣으면 안됨
            j++;
        }
        else {
            //A교집합B 넣어야함
            result.append(this->array[i]);
            i++;
            j++;
        }
    }
    return result;
}

MySet MySet::differenceSet(const MySet &set) const{
    MySet result;
    int i = 0, j = 0;

    while (i < this->size && j < set.size) {
        if (this->array[i] < set.array[j]) {
            //A append
            result.append(this->array[i++]);
        } else if (this->array[i] > set.array[j]) {
            //B는 넣으면 안됨
            j++;
        } else {
            //A교집합B 넣으면 안됨
            i++;
            j++;
        }
    }
    //A 나머지 append
    while (i < this->size) {
        result.append(this->array[i++]);
    }

    return result;
}

void MySet::append(const int val) {
    if (size >= capacity) {
        resize();
    }
    array[size++] = val;
}
