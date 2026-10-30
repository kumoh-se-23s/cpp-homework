#include "MySet.h"
#include <iomanip>

// =================================================================
// PUBLIC SECTION
// =================================================================

MySet::MySet(){
    this->array = new int[DEFAULT_CAPACITY];
}

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

//insert면 val, pos 받아야할 거 같은 뭔가 그런 느낌인데 흠,,
void MySet::insert(const int value) {
    const int pos = findPos(value);
    //중복 제거
    if (pos < size && array[pos] == value) {
        return;
    }
    insertAt(pos, value);
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

    //A, B를 정렬으로 받음 >> merge와 비슷하게 Union 구현
    while (i < this->size && j < set.size) {
        if (this->array[i] < set.array[j]) {
            //A - B append
            result.append(this->array[i++]);
        } else if (this->array[i] > set.array[j]) {
            //B - A append
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
            //A - B 넣으면 안됨
            i++;
        } else if (this->array[i] > set.array[j]) {
            //B - A 넣으면 안됨
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
            //A - B append
            result.append(this->array[i++]);
        } else if (this->array[i] > set.array[j]) {
            //B - A 는 넣으면 안됨
            j++;
        } else {
            //A교집합B도 넣으면 안됨
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
//based on binary search
int MySet::findPos(const int val) const {
    int low = 0, high = size - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (array[mid] > val) {
            high = mid - 1;
        }
        else if (array[mid] < val) {
            low = mid + 1;
        }
        else {
            return mid;
        }
    }
    return low;
}

void MySet::insertAt(const int pos, const int val) {
    if (size >= capacity) {
        resize();
    }
    for (int k = size; k > pos; k--) {
        array[k] = array[k - 1];
    }
    array[pos] = val;
    size++;
}
