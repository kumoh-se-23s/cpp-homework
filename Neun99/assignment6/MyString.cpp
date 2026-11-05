#include "MyString.h"
#include <iostream>
#include <limits>

//생성자
MyString::MyString() {
    str = new char[DEFAULT_CAPACITY];
    size = 0;
    str[size] = '\0';
}

//복사생성자: str
MyString::MyString(const MyString &srcStr) {
    size = srcStr.size;
    capacity = getEnoughCapacity(size); //기본값에서 2배씩 늘려가며 최소용량 잡기
    str = new char[capacity];
    copyArr(str, srcStr.str, srcStr.size);
}

//복사생성자: cArr
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

//용량 반환
int MyString::getCapacity() const {
    return capacity;
}

//pos위치 주소 반환
char& MyString::at(int pos) {
    if (pos < 0 || pos >= size)
        return str[size];
    return str[pos];
}

//pos위치 주소 반환
const char& MyString::at(int pos) const {
    if (pos < 0 || pos >= size)
        return str[size];
    return str[pos];
}

//find: cArr
int MyString::find(const char *cSubStr, int pos) const {
    pos = normalizePos(pos);
    return findChArr(cSubStr, getLengthOfChArr(cSubStr), pos);
}

//find: str
int MyString::find(const MyString &subStr, int pos) const {
    pos = normalizePos(pos);
    return findChArr(subStr.str, subStr.size, pos);
}

//문자열 부분추출
MyString MyString::substr(int pos, int len) const {
    //입력정리
    if (pos < -size || pos > size || len < 0) //범위 외 pos면 빈 문자열 리턴
        return MyString();

    if (pos < 0) { //범위 내 음수면 뒷글자부터 역행
        pos = size + pos; //역행시작점 (오른쪽 끝)
        int startIdx = pos - len + 1; //역행 도착점 (왼쪽 끝, 오른쪽 끝이 자기 포함이니 여기서 + 1)
        if (startIdx < 0) //도착점이 범위를 넘어서면 문자열 처음까지만
            startIdx = 0;
        len = pos - startIdx + 1; //길이 = 역행 시작점 - 도착점 + 1
        pos = startIdx; //Impl에 넣기 위해 변경
    } else {
        len = normalizeLen(len, pos);
    }

    return subStrImpl(pos, len);
}

//append: cArr
MyString &MyString::append(const char *cSubStr) {
    appendChArr(cSubStr, getLengthOfChArr(cSubStr));
    return *this;
}

//append: str
MyString &MyString::append(const MyString &subStr) {
    appendChArr(subStr.str, subStr.size);
    return *this;
}

//insert: cArr
MyString &MyString::insert(int pos, const char *csubStr) {
    pos = normalizePos(pos);
    insertChArr(csubStr, getLengthOfChArr(csubStr), pos);
    return *this;
}

//insert: str
MyString &MyString::insert(int pos, const MyString &subStr) {
    pos = normalizePos(pos);
    insertChArr(subStr.str, subStr.size, pos);
    return *this;
}

//replace: cArr
MyString &MyString::replace(int pos, int len, const char *csubStr) {
    pos = normalizePos(pos);
    len = normalizeLen(len, pos);
    replaceChArr(csubStr, getLengthOfChArr(csubStr), pos, len);
    return *this;
}

//replace: str
MyString &MyString::replace(int pos, int len, const MyString &subStr) {
    pos = normalizePos(pos);
    len = normalizeLen(len, pos);
    replaceChArr(subStr.str, subStr.length(), pos, len);
    return *this;
}

//erase
MyString &MyString::erase(int pos, int cnt) {
    pos = normalizePos(pos);
    cnt = normalizeLen(cnt, pos);
    //pos부터 idx + cnt를 idx로 옮김
    for (int idx = pos; idx + cnt < size; idx++) {
        str[idx] = str[idx + cnt];
    }
    size -= cnt;
    str[size] = '\0';
    return *this;
}

//비우기
void MyString::clear() {
    size = 0;
    str[size] = '\0';
}

//stoi용: 진수에 맞는 값인지 판별 (base 범위 2~36)
static bool isValidValue(char ch, int base) {
    if (base <= 10)
        return ch >= '0' && ch < '0' + base;
    return (ch >= '0' && ch < '0' + base) || (ch >= 'a' && ch < 'a' + base - 10);
}

//stoi용: 단일문자 숫자화 (범위 0~9, a~z)
static int toInt(char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    return ch - 'a' + 10;
}

//진수 읽기
int stoi(const MyString &str, int pos, int base) {
    //인자 정리
    if (base < 2 || base > 36)
        return 0;

    if (pos < 0)
        pos = 0;
    else if (pos > str.length())
        pos = str.length();

    int result = 0;
    for (int idx = pos; idx < str.length() && isValidValue(str[idx], base); idx++) {
        result = result * base + toInt(str[idx]);
    }
    return result;
}

//연산자 오버로딩--------------------
//+ cArr
const MyString MyString::operator+(const char *cstr2nd) const {
    MyString result = *this;
    result.appendChArr(cstr2nd, getLengthOfChArr(cstr2nd));
    return result;
}

//+ str
const MyString MyString::operator+(const MyString &str2nd) const {
    MyString result = *this;
    result.appendChArr(str2nd.str, str2nd.size);
    return result;
}

//+= cArr
MyString &MyString::operator+=(const char *cstr2nd) {
    return append(cstr2nd);
}

//+= str
MyString &MyString::operator+=(const MyString &str2nd) {
    return append(str2nd);
}

//= cArr
MyString &MyString::operator=(const char *cstr2nd) {
    int cLength = getLengthOfChArr(cstr2nd);
    substitute(cstr2nd, cLength);
    return *this;
}

//= str
MyString &MyString::operator=(const MyString &srcStr) {
    if (this != &srcStr) //자신이면 대입x
        substitute(srcStr.str, srcStr.size);
    return *this;
}

//[]
char &MyString::operator[](int pos) {
    return at(pos);
}

//[]
const char &MyString::operator[](int pos) const {
    return at(pos);
}

//== cArr
bool MyString::operator==(const char *cstr2nd) const {
    int chArrLength = getLengthOfChArr(cstr2nd);
    if (size != chArrLength)
        return false;
    return compareTo(cstr2nd, chArrLength) == 0;
}

//== str
bool MyString::operator==(const MyString &str2nd) const {
    if (size != str2nd.size)
        return false;
    return compareTo(str2nd.str, str2nd.size) == 0;
}

//!= cArr
bool MyString::operator!=(const char *cstr2nd) const {
    return !(*this == cstr2nd);
}

//!= str
bool MyString::operator!=(const MyString &str2nd) const {
    return !(*this == str2nd);
}

//< cArr
bool MyString::operator<(const char *cstr2nd) const {
    return compareTo(cstr2nd, getLengthOfChArr(cstr2nd)) == -1;
}

//< str
bool MyString::operator<(const MyString &str2nd) const {
    return compareTo(str2nd.str, str2nd.size) == -1;
}

//> cArr
bool MyString::operator>(const char *cstr2nd) const {
    return compareTo(cstr2nd, getLengthOfChArr(cstr2nd)) == 1;
}

//> str
bool MyString::operator>(const MyString &str2nd) const {
    return compareTo(str2nd.str, str2nd.size) == 1;
}

//<= cArr
bool MyString::operator<=(const char *cstr2nd) const {
    return compareTo(cstr2nd, getLengthOfChArr(cstr2nd)) <= 0;
}

//<= str
bool MyString::operator<=(const MyString &str2nd) const {
    return compareTo(str2nd.str, str2nd.size) <= 0;
}

//>= cArr
bool MyString::operator>=(const char *cstr2nd) const {
    return compareTo(cstr2nd, getLengthOfChArr(cstr2nd)) >= 0;
}

//>= str
bool MyString::operator>=(const MyString &str2nd) const {
    return compareTo(str2nd.str, str2nd.size) >= 0;
}

ostream &operator<<(ostream &out, const MyString &str) {
    for (int idx = 0; idx < str.length(); idx++) {
        out << str[idx];
    }
    return out;
}

//whitespace 판별
static bool isWhiteSpace(int chCode) {
    return chCode == ' ' || chCode == '\n' || chCode == '\t' || chCode == '\r';
}

//입력
istream& operator>>(istream& in, MyString& str) {
    str.clear(); //초기화

    //stream 앞부분 정리
    int inputCode = in.peek();
    for (;isWhiteSpace(inputCode); inputCode = in.peek()) {
        in.ignore();
    }

    //이후 append
    for (char ch; !isWhiteSpace(inputCode) && inputCode != EOF; inputCode = in.peek()) {
        in.get(ch);
        str.appendCh(ch);
    }
    return in;
}

//getline
istream& getline(istream& in, MyString& str, char delim) {
    str.clear(); //초기화

    for (int inputCode = in.peek(); inputCode != delim && inputCode != EOF; inputCode = in.peek()) {
        char ch;
        in.get(ch);
        str.appendCh(ch);
    }

    in.ignore(numeric_limits<streamsize>::max(),delim);
    return in;
}

//private--------------------------
//resize: 인자는 최소 길이, capacity는 항상 8 * 2^n배수
void MyString::resize(int minLength) {
    capacity = getEnoughCapacity(minLength);
    char *newArr = new char[capacity];

    copyArr(newArr, str, size);
    delete[] str;
    str = newArr;
}

//배열 깊은복사
void MyString::copyArr(char *copy, const char *src, int newSize) {
    for (int idx = 0; idx < newSize; idx++) {
        copy[idx] = src[idx];
    }
    copy[newSize] = '\0';
}

//배열 길이 구하기
int MyString::getLengthOfChArr(const char *chArr) {
    int idx;
    for (idx = 0; chArr[idx] != '\0'; idx++) {}
    return idx;
}

//인자로 최소길이 받아서, capacity 두 배로 늘리면서 적절한 용량 구하기
int MyString::getEnoughCapacity(int minLength) const {
    int result;
    for (result = capacity; result <= minLength; result *= 2) {}
    return result;
}

//pos가 0 미만이면 0, size보다 크면 size, 아니면 그대로 리턴
int MyString::normalizePos(int pos) const{
    if (pos < 0)
        return 0;
    if (pos > size)
        return size;
    return pos;
}

int MyString::normalizeLen(int len, int pos) const {
    if (len > size - pos || len < 0)
        return size - pos;
    return len;
}

//대입 동작부
void MyString::substitute(const char *chArr, int newSize) {
    //크기 감당 안되면 재할당
    if (newSize >= capacity) {
        capacity = getEnoughCapacity(newSize);
        delete[] str;
        str = new char[capacity];
    }
    copyArr(str, chArr, newSize);
    size = newSize;
}

//find 동작부
int MyString::findChArr(const char *chArr, int arrSize, int pos) const{
    if (arrSize == 0)
        return pos;
    if (size - pos < arrSize) //길이 기반 판단했을 때 절대 못 찾는 경우
        return -1;
    for (int idx = pos; idx <= size - arrSize; idx++) {
        if (str[idx] == chArr[0]) { //첫글자 같으면 거기부터 비교
            bool isFound = true; //flag
            for (int subIdx = 0; isFound && subIdx < arrSize; subIdx++) {
                //안 맞으면 flag 넘어뜨리고 탈출
                if (str[idx + subIdx] != chArr[subIdx])
                    isFound = false;
            }
            if (isFound)
                return idx;
        }
    }
    return -1;
}

//subStr 동작부
MyString MyString::subStrImpl(int pos, int len) const {
    MyString result;
    if (len >= DEFAULT_CAPACITY)
        result.resize(len);
    for (int idx = 0; idx < len; idx++) {
        result.str[idx] = str[idx + pos];
    }
    result.size = len;
    result.str[result.size] = '\0';

    return result;
}

//한 글자 붙이기
void MyString::appendCh(char ch) {
    if (size + 1>= capacity)
        resize(size + 1);
    str[size++] = ch;
    str[size] = '\0';
}

//apeend 동작부
void MyString::appendChArr(const char *chArr, int chArrSize) {
    if (size + chArrSize >= capacity)
        resize(size + chArrSize);

    for (int idx = 0; idx < chArrSize; idx++) {
        str[idx + size] = chArr[idx];
    }
    size += chArrSize;
    str[size] = '\0';
}

//insert 동작부
void MyString::insertChArr(const char *chArr, int chArrSize, int pos) {
    if (size + chArrSize >= capacity)
        resize(size + chArrSize);

    //원래 글자를 뒤로 밀기
    for (int idx = size - 1; idx >= pos; idx--) {
        str[idx + chArrSize] = str[idx];
    }
    //insert
    for (int idx = pos; idx < pos + chArrSize; idx++) {
        str[idx] = chArr[idx - pos];
    }

    size += chArrSize;
    str[size] = '\0';
}

//replace 동작부
void MyString::replaceChArr(const char *chArr, int chArrSize, int pos, int len) {
    erase(pos, len);
    insertChArr(chArr, chArrSize, pos);
}

//compareTo용 더 작은 수 리턴
int MyString::min(int val1, int val2) {
    if (val1 <= val2)
        return val1;
    return val2;
}

//사전순 대소비교
int MyString::compareTo(const char *chArr, int chArrSize) const {
    int range = min(size, chArrSize); //길이 짧은 쪽의 \0까지 비교
    for (int idx = 0; idx <= range; idx++) {
        if (str[idx] > chArr[idx])
            return 1;
        if (str[idx] < chArr[idx])
            return -1;
    }
    return 0;
}