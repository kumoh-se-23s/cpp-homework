#include <iostream>
#include "MySet.h"

using namespace std;

//생성자
MySet::MySet() {
    arr = new int[DEFAULT_CAPACITY];
    capacity = DEFAULT_CAPACITY;
    size = 0;
}

//복사생성자
MySet::MySet(const MySet &set) {
    arr = new int[set.capacity];
    copyArr(arr, set.arr, set.size);
    capacity = set.capacity;
    size = set.size;
}

//소멸자
MySet::~MySet() {
    delete[] arr;
}

//getter ------------------------------------
//사용크기 반환
int MySet::getSize() const {
    return size;
}

//pos의 값 반환
int MySet::getValue(int pos) const {
    if (pos >= size || pos < 0)
        return -1;
    return arr[pos];
}

//기능 --------------------------------------
//원소 추가: 이진 탐색 기반 동작
void MySet::insert(int value) {
    int targetIdx = findInsertPos(value); //이진 탐색 실패 시 lowIdx = 넣을 자리
    if (targetIdx != -1)
        insertAt(targetIdx, value);
}

//청소
void MySet::clear() {
    delete[] arr;
    arr = new int[DEFAULT_CAPACITY];
    capacity = DEFAULT_CAPACITY;
    size = 0;
}

//연산자 오버로딩 ---------------------------------
//합집합 (merge)
const MySet MySet::operator+(const MySet &set2) const {
    MySet result;
    //오름차순임을 전제로, 작은 숫자부터 점점 올려가며 비교
    int set1Idx = 0, set2Idx = 0;
    while (set1Idx < size && set2Idx < set2.size) {
        int set1Val = arr[set1Idx];
        int set2Val = set2.arr[set2Idx];

        if (set1Val == set2Val) { //같으면 한번만 append, 둘 다 ++
            result.append(set1Val);
            set1Idx++;
            set2Idx++;
        } else if (set1Val > set2Val) { //한 쪽이 작으면 작은 쪽만 더하고 ++
            result.append(set2Val);
            set2Idx++;
        } else {
            result.append(set1Val);
            set1Idx++;
        }
    }

    //아래 for문은 둘 중 하나만 동작
    for (;set1Idx < size; set1Idx++) {
        result.append(arr[set1Idx]);
    }
    for (;set2Idx < set2.size; set2Idx++) {
        result.append(set2.arr[set2Idx]);
    }

    return result;
}

//교집합
const MySet MySet::operator&(const MySet &set2) const {
    MySet result;

    //+와 같은 구조로 전진, append 조건만 다름
    for (int set1Idx = 0, set2Idx = 0; set1Idx < size && set2Idx < set2.size;) {
        int set1Val = arr[set1Idx];
        int set2Val = set2.arr[set2Idx];

        if (set1Val == set2Val) { //같은값이면 append
            result.append(arr[set1Idx]);
            set1Idx++;
            set2Idx++;
        } else if (set1Val > set2Val) { //다르면 그냥 지나감
            set2Idx++;
        } else {
            set1Idx++;
        }
    }
    return result;
}

//차집합
const MySet MySet::operator-(const MySet &set2) const {
    MySet result;

    int set1Idx = 0;
    for (int set2Idx = 0; set1Idx < size && set2Idx < set2.size;) {
        int set1Val = arr[set1Idx];
        int set2Val = set2.arr[set2Idx];

        if (set1Val == set2Val) {
            set1Idx++;
            set2Idx++;
        } else if (set1Val > set2Val) {
            set2Idx++;
        } else { //오름차순이므로 자기값보다 상대가 더 크면 그 값은 상대에 존재하지 않음
            result.append(set1Val);
            set1Idx++;
        }
    }

    //상대가 먼저 끝났으면 남은 원소 밀어넣기
    for (; set1Idx < size; set1Idx++) {
        result.append(arr[set1Idx]);
    }
    return result;
}

//대입
MySet& MySet::operator=(const MySet &set2) {
    if (this == &set2)
        return *this;

    delete[] arr;
    arr = new int[set2.capacity];
    copyArr(arr, set2.arr, set2.size);
    capacity = set2.capacity;
    size = set2.size;

    return *this;
}

//<<
ostream &operator<<(ostream &out, const MySet &set) {
    out << "{";
    if (set.getSize() != 0)
        out << set.getValue(0);
    for (int idx = 1; idx < set.getSize(); idx++) {
        out << ", " << set.getValue(idx);
    }
    out << "}";
    return out;
}

//>>
istream &operator>>(istream &in, MySet &set) {
    set.clear();
    int value;
    for (in >> value; value >= 0; in >> value) {
        set.insert(value);
    }
    return in;
}

//private-------------------------------------
//크기 두배
void MySet::resize() {
    capacity *= 2;
    int *newArr = new int[capacity];

    copyArr(newArr, arr, size);

    delete[] arr;
    arr = newArr;
}

//배열 복사
void MySet::copyArr(int *copy, const int *src, int srcSize) {
    for (int idx = 0; idx < srcSize; idx++) {
        copy[idx] = src[idx];
    }
}

//이진탐색으로 넣을 위치 판단, 중복 값 있으면 -1
int MySet::findInsertPos(int value) const {
    int lowIdx = 0;
    int highIdx = size - 1;
    while (lowIdx <= highIdx) {
        int midIdx = lowIdx + ((highIdx - lowIdx) / 2);
        if (arr[midIdx] == value)
            return -1;
        if (arr[midIdx] < value)
            lowIdx = midIdx + 1;
        else
            highIdx = midIdx - 1;
    }
    return lowIdx;
}
//원소 추가: 그냥 갖다붙임
void MySet::append(int value) {
    insertAt(size, value);
}

//원소 추가: pos에 넣음
void MySet::insertAt(int pos, int value) {
    //용량 체크
    if (size + 1 > capacity)
        resize();

    //한칸씩 뒤로 밀기
    for (int idx = size; idx >= pos + 1; idx--) {
        arr[idx] = arr[idx - 1];
    }

    //처리된 자리에 넣음
    arr[pos] = value;
    size++;
}