#include <iostream>
#include "MySet.h"

using namespace std;

MySet::MySet() {
    arr = new int[DEFAULT_CAPACITY];
    capacity = DEFAULT_CAPACITY;
    size = 0;
}

MySet::~MySet() {
    delete[] arr;
}

//getter ------------------------------------
int MySet::getSize() const {
    return size;
}

int MySet::getValue(int pos) const {
    if (pos > size || pos < 0)
        return -1;
    return arr[pos];
}

//기능 --------------------------------------
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

//원소 추가: insertion sort 기반 동작
void MySet::insert(int value) {
    if (size == 0) { //빈 배열이면 그냥 넣고 early return
        arr[0] = value;
        size++;
        return;
    }

    bool isInserted = false;
    for (int targetIdx = 0; !isInserted && targetIdx < size; targetIdx++) {
        if (arr[targetIdx] == value) //같은 값 있으면 넣지 않음
            isInserted = true;

        if (arr[targetIdx] > value) { //value보다 큰 값 찾으면 그 자리에 insert
            //용량 체크
            if (size + 1 >= capacity)
                resize();

            //한칸씩 앞으로 밀기
            for (int idx = size; idx >= targetIdx; idx--) {
                arr[size] = arr[size - 1];
            }

            //빈 자리에 insert 후 종료
            arr[targetIdx] = value;
            isInserted = true;
        }
    }
    if (!isInserted) {
        if (size + 1 >= capacity)
            resize();
        arr[size] = value;
        size++;
    }
}

//원소 추가: 그냥 갖다붙임 (시간복잡도 줄이는용)
void MySet::append(int value) {
    if (size + 1 >= capacity)
        resize();
    arr[++size] = value;
}

//연산자 오버로딩 ---------------------------------
//합집합
const MySet MySet::operator+(const MySet &set2) const {
    MySet result = *this;

    for (int idx = 0; idx < set2.size; idx++) {
        result.insert(set2.arr[idx]);
    }

    return result;
}

//교집합
const MySet MySet::operator&(const MySet &set2) const {
    MySet result;

    //오름차순임을 전제로, 작은 숫자부터 점점 올려가며 비교
    for (int set1Idx = 0, set2Idx = 0; set1Idx < size && set2Idx < set2.size;) {
        int set1Val = arr[set1Idx];
        int set2Val = set2.arr[set2Idx];

        if (set1Val == set2Val) { //같은값이면 result에 append
            result.append(arr[set1Idx]);
            set1Idx++;
            set2Idx++;
        } else if (set1Val > set2Val) { //다르면 값이 작은 쪽의 idx를 ++
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

    for (int idx = 0; idx < size; idx++) {
        //arr[idx]가 set2에 없는 값일 때만 append
        for (int set2Idx = 0; arr[idx] != set2.arr[set2Idx] && set2Idx < size; set2Idx++) {
            //오름차순 특성상 자신이 더 커지면 그 값이 없다는 뜻이므로 무의미한 반복 그만두고 append 후 탈출
            if (arr[idx] > set2.arr[set2Idx]) {
                result.append(arr[idx]);
                break;
            }
        }
    }
}

//대입
MySet MySet::operator=(const MySet &set2) {
    if (*arr == *set2.arr)
        return *this;

    int *newArr = new int[set2.capacity];
    copyArr(newArr, set2.arr, set2.size);

    delete[] arr;
    arr = newArr;
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
    int value;
    for (in >> value; value >= 0; in >> value) {
        set.insert(value);
    }
    return in;
}
