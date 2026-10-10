#include "MySet.h"

#include <array>

MySet::MySet() = default;

MySet::~MySet() {
    delete [] array;
    array = nullptr;
}

//연산자 오버로딩

MySet MySet::operator+(const MySet& set) const{
    return this->unionSet(set);
}

MySet MySet::operator-(const MySet& set) const{
    return this->differenceSet(set);

}

MySet MySet::operator&(const MySet& set) const{
    return this->intersectionSet(set);
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

//private 영역

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

    //현재 먼저 넣기
    for (int i = 0; i < this->size; i++) {
        result.insert(this->array[i]);
    }
    //나머지 붙히기
    for (int i = 0; i < set.size; i++) {
        result.insert(set.array[i]);
    }
    return result;
}

MySet MySet::intersectionSet(const MySet &set) const{
    MySet result;

    //일단 바로 생각나는건 전체 확인인데 더 최적화 없으려나
    for (int i = 0; i < this->size; i++) {
        int target = this->array[i];
        for (int j = 0; j < set.size; j++) {
            if (target == set.array[j]) {
                result.insert(target);
                break;
            }
        }
    }

    return result;
}

MySet MySet::differenceSet(const MySet &set) const{

}

//정렬이랑 중복검사 같이하며 값을 넣음
void MySet::insert(int value) {
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
