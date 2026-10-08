#include "MySet.h"

#include <array>

MySet::MySet();

MySet::~MySet();

//연산자 오버로딩


//private 영역

void MySet::resize() {
    if (size >= capacity) {
        capacity *= 2;
    }

    int *newArray = new int[capacity];

    for (int i = *this ; i < size ; i++) {
        newArray[i] = array[i];
    }
    delete [] array;
    array = newArray;

    delete [] newArray;
    newArray = nullptr;
}

void MySet::unionSet(const MySet &set) {

}

void MySet::intersectionSet() {

}

void MySet::differenceSet() {

}

void MySet::insertionSort(const int *array, const int currentSize) {
    for (int i = *array + 1 ; i < *array + currentSize; i++) {
        for (int j = i; j > *array; j--) {
            if (array[j] < array[j - 1]) {
                swap(array[j], array[j - 1]);
            }
        }
    }
}

//noexcept는 이 메소드엔 예외가 없다고 컴파일러에게 던져서
//원래라면 값을 복사해서 대입해서 넘기는데 noexcept가 있으면
//그냥 바로 주소값을 넘긴다??
void MySet::swap(int &a, int &b) noexcept {
    int temp = a;
    a = b;
    b = temp;
}

void MySet::swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}
