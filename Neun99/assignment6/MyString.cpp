#include "MyString.h"

MyString::MyString() {
    str = new char[DEFAULT_CAPACITY];
    //빈 문자열 초기화
    for (int idx = 0; idx < DEFAULT_CAPACITY; idx++) {
        str[idx] = '\0';
    }
    capacity = DEFAULT_CAPACITY;
    size = 0;
}


//private--------------------------
void MyString::resize() {
    capacity *= 2;
    char *newArr = new char[capacity];

    copyArr(newArr, str, size);

}

void MyString::copyArr(char *copy, char *src, int size) {
    for (int idx = 0; idx < size; idx++) {
        copy[idx] = src[idx];
    }
}