#include "MyString.h"
#include <stdexcept>
#include <limits>

MyString::MyString() {
    str = new char[DEFAULT_CAPACITY];
    //빈 문자열 초기화
    for (int idx = 0; idx < DEFAULT_CAPACITY; idx++) {
        str[idx] = '\0';
    }
    capacity = DEFAULT_CAPACITY;
    size = 0;
}

//복사생성자
MyString::MyString(const MyString &srcStr) {
    str = new char[srcStr.capacity];
    copyArr(str, srcStr.str, srcStr.size);
    capacity = srcStr.capacity;
    size = srcStr.size;
}

//복사생성자2
MyString::MyString(const char *cstr2nd) {
    size = getLengthOfChArr(cstr2nd);
    capacity = getEnoughCapacity(size);
    str = new char[capacity];
    copyArr(str, cstr2nd, size);
}

//소멸자
MyString::~MyString() {
    delete[] str;
}

//기능-----------------------------
//길이 반환
int MyString::length() const{
    return size;
}

//비었는지 반환
bool MyString::isEmpty() const{
    return size == 0;
}

int MyString::getCapacity() const {
    return capacity;
}

char& MyString::at(int pos) const{
    if (pos < 0 || pos >= size)
        return '\0';
    return str[pos];
}

int MyString::find(const char *cSubStr, int pos) const {
    return findChArr(cSubStr, getLengthOfChArr(cSubStr), pos);
}

int MyString::find(const MyString &subStr, int pos) const {
    return findChArr(subStr.str, subStr.size, pos);
}

MyString MyString::subStr(int pos, int len) const {
    if (len < 0 || len > size)
        len = numeric_limits<int>::max();
    if (pos >= 0)
        return subStrImpl(pos, len);
    return subStrImpl(size + pos, len); //음수면 뒷글자
}

MyString &MyString::append(const char *cSubStr) {

}

//연산자 오버로딩--------------------

//private--------------------------
void MyString::resize() {
    capacity *= 2;
    char *newArr = new char[capacity];

    copyArr(newArr, str, size);
    delete[] str;
    str = newArr;
}

void MyString::copyArr(char *copy, const char *src, int newSize) {
    for (int idx = 0; idx < newSize; idx++) {
        copy[idx] = src[idx];
    }
    copy[newSize] = '\0';
}

int MyString::getLengthOfChArr(const char *chArr) {
    int idx;
    for (idx = 0; chArr[idx] != '\0'; idx++) {}
    return idx + 1;
}

int MyString::getEnoughCapacity(int size) {
    int result;
    for (result = DEFAULT_CAPACITY; result < size; result *= 2) {}
    return result;
}

void MyString::substitute(const char *chArr, int newCapacity, int newSize) {
    delete[] str;
    str = new char[newCapacity];
    copyArr(str, chArr, newSize);

    capacity = newCapacity;
    size = newSize;
}

int MyString::findChArr(const char *chArr, int arrSize, int pos) const{
    if (pos < 0) //pos 음수시 0
        pos = 0;
    if (pos > size) //범위 밖이면 마지막 위치
        pos = size - 1;
    if (arrSize == 0 || size - pos < arrSize) //길이 기반 판단했을 때 절대 못 찾는 경우
        return -1;
    for (int idx = pos; idx < size - arrSize; idx++) {
        if (str[idx] == chArr[0]) { //첫글자 같으면 거기부터 비교
            bool isFound = true; //flag
            for (int subIdx = 0; subIdx < arrSize; subIdx++) {
                //안 맞으면 flag 넘어뜨리고 탈출
                if (str[idx + subIdx] != chArr[subIdx]) {
                    isFound = false;
                    break;
                }
            }
            if (isFound)
                return idx;
        }
    }
    return -1;
}

const MyString MyString::subStrImpl(int pos, int len) const {
    MyString resultStr;
    for (int idx = 0; idx < len; idx++) {
        resultStr.str[idx] = str[idx + pos];
    }
    resultStr.size = len;
    resultStr.str[resultStr.size] = '\0';

    return resultStr;
}

void MyString::appendCh(char ch) {
    if (size + 1 > capacity)
        resize();
    str[size++] = ch;
    str[size] = '\0';
}

void MyString::appendChArr(const char *chArr, int chArrSize) {
    capacity = getEnoughCapacity(size + chArrSize);
    char *newArr = new char[capacity];
    copyArr(newArr, str, size);
    for (int idx = 0; idx < chArrSize; idx++) {
        newArr[idx + size] = chArr[idx];
    }
    size += chArrSize;
    newArr[size] = '\0';

    delete[] str;
    str = newArr;
}
